#pragma once
#include "pch/pch.h"
#include "minhook/MinHook.h"
#include "minhook/hde/hde64.h"
#include "minhook/trampoline.h"

class MinHook {
public:
	MinHook() { MH_Initialize(); }
	~MinHook() { MH_Uninitialize(); }
public:
	template <typename T>
	static bool Create(T Target, T Detour, T& Originial) {
		if (!Target || !Detour) {
			return false;
		}
		MH_STATUS Status{ MH_CreateHook(Target, LPVOID(Detour), (LPVOID*)&Originial) };
		if (Status == MH_STATUS::MH_OK) {
			return true;
		}
		if (Status != MH_STATUS::MH_ERROR_NOT_INITIALIZED && Status != MH_STATUS::MH_ERROR_UNSUPPORTED_FUNCTION && Status != MH_STATUS::MH_ERROR_MEMORY_ALLOC && Status != MH_STATUS::MH_ERROR_MEMORY_PROTECT) {
			if (std::optional<LPVOID> CorrectedTarget{ GetCorrectedTarget<T>(Target) }; CorrectedTarget.has_value()) {
				Status = MH_CreateHook(CorrectedTarget.value(), LPVOID(Detour), (LPVOID*)&Originial);
				if (Status == MH_STATUS::MH_OK) {
					return true;
				}
				return false;
			}
		}
		return false;
	}
	template <typename T>
	static bool Destroy(T Target) {
		return MH_RemoveHook(LPVOID(Target)) == MH_STATUS::MH_OK;
	}
	template <typename T>
	static bool QueueEnable(T Target) {
		return MH_QueueEnableHook(LPVOID(Target)) == MH_STATUS::MH_OK;
	}
	template <typename T>
	static bool QueueDisable(T Target) {
		return MH_QueueDisableHook(LPVOID(Target)) == MH_STATUS::MH_OK;
	}
	static bool Apply() {
		return MH_ApplyQueued() == MH_STATUS::MH_OK;
	}
public:
	template <typename T>
	static std::optional<LPVOID> GetCorrectedTarget(T& Target) {
		MEMORY_BASIC_INFORMATION mi{};
		VirtualQuery((void*)Target, &mi, sizeof(mi));
		if (!(mi.State == MEM_COMMIT && (mi.Protect & PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) {
			return {}; //Code is not excutable, just say fuck it and make it take one for the team
		}
		mem ptr{ (void*)Target };
		while (ptr.as<u8&>() == 0xE9) { //Jmp over detours
			ptr = ptr.call();
		}
		//Find out how much we should offset the function based on the tramopline
		u8 oPos{}, nPos{};
		u64 dest{};
		bool done{};
		int runCount{};
		do {
			if (runCount <= 10) {
				nPos = NULL; //We were unable to determine the tramopline size
				done = true;
			}
			hde64s ctx{};
			u32 size{ hde64_disasm(ptr.as<PVOID>(), &ctx) };
			if (oPos >= sizeof(JMP_REL)) {
				size = sizeof(JMP_ABS);
				done = true;
			}
			else if ((ctx.modrm & 0xC7) == 0x05) {
				if (ctx.opcode == 0xFF && ctx.modrm_reg == 4)
					done = true;
			}
			else if (ctx.opcode == 0xE8) {
				size = sizeof(CALL_ABS);
			}
			else if ((ctx.opcode & 0xFD) == 0xE9) {
				//Direct relative JMP (EB or E9)
				u64 dest{ (u64)Target + ctx.len };
				if (!(u64)Target <= dest && dest < ((u64)Target + sizeof(JMP_REL))) {
					size = sizeof(JMP_ABS);
					done = true;
				}
			}
			else if ((ctx.opcode & 0xF0) == 0x70 || (ctx.opcode & 0xFC) == 0xE0 || (ctx.opcode2 & 0xF0) == 0x80) {
				if ((ctx.opcode & 0xFC) == 0xE0) {
					done = true;
				}
				else {
					done = true;
					size = sizeof(JCC_ABS);
				}
			}
			else if ((ctx.opcode & 0xFE) == 0xC2) {
				done = true;
			}
			nPos += size;
			oPos += ctx.len;
			runCount++;
		} while (!done);
		if (nPos)
			ptr = ptr.add(nPos);
		else
			ptr = ptr.add(5);
		return ptr.as<PVOID>();
	}
};
class detour {
public:
	detour(ccp name, void* ptr, void* dtr, bool hook = true) : m_name(name), m_ptr(ptr), m_og(ptr), m_dtr(dtr), m_hook(hook) {
		if (m_hook) {
			MinHook::Create(m_ptr, m_dtr, m_og);
		}
	}
	~detour() {
		if (m_hook) {
			MinHook::Destroy(m_ptr);
		}
	}
public:
	void enable() {
		if (m_hook) {
			MinHook::QueueEnable(m_ptr);
		}
	}
	void disable() {
		if ( m_hook) {
			MinHook::QueueDisable(m_ptr);
		}
	}
public:
	inline static void enableAll() {
		MH_QueueEnableHook(MH_ALL_HOOKS);
	}
	inline static void disableAll() {
		MH_QueueDisableHook(MH_ALL_HOOKS);
	}
public:
	template <typename t>
	t getOg() {
		return static_cast<t>(m_og);
	}
public:
	ccp m_name{};
	void* m_dtr{};
	void* m_ptr{};
	void* m_og{};
	bool m_hook{};
};