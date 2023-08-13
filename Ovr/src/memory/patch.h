#pragma once
#include "pch/pch.h"
#include "jitasm/jitasm.h"

template <typename t>
inline void mcpy(t address, t data, u64 size) {
	if (!address) {
		return;
	}
	u32 oldProt{};
	VirtualProtect((void*)address, size, PAGE_EXECUTE_READWRITE, (DWORD*)&oldProt);
	memcpy((void*)address, (void*)data, size);
	VirtualProtect((void*)address, size, oldProt, (DWORD*)&oldProt);
}
template <typename t>
inline void mset(t address, i32 value, i32 size) {
	if (!address) {
		return;
	}
	u32 oldProt{};
	VirtualProtect((void*)address, size, PAGE_EXECUTE_READWRITE, (DWORD*)&oldProt);
	memset((void*)address, value, size);
	VirtualProtect((void*)address, size, oldProt, (DWORD*)&oldProt);
}
template <typename a, typename v>
inline void put(a addr, v value) {
	memcpy(reinterpret_cast<void*>(addr), &value, sizeof(value));
}
template<typename a>
inline void nop(a addr, i32 size) {
	mset<a>(addr, 0x90, size);
}
template <typename a, typename t>
inline void call(a addr, t func) {
	put<a, u8>(addr, 0xE8);
	put<u64, i32>(reinterpret_cast<u64>(addr) + 1, reinterpret_cast<u64>(func) - reinterpret_cast<u64>(addr) - 5);
}
class patch {
public:
	patch(cc* id, u8* ptr, const std::vector<u8>& bytes, bool set = false) : m_id(id), m_ptr(ptr), m_original(bytes), m_bytes(bytes), m_set(set)  {
		mcpy(m_original.data(), m_ptr, m_bytes.size());
	}
	~patch() {
		restore();
	}
public:
	void apply() {
		mcpy(m_ptr, m_bytes.data(), m_bytes.size());
	}
	void restore() {
		mcpy(m_ptr, m_original.data(), m_original.size());
	}
	template <typename t>
	t* get() {
		return reinterpret_cast<t*>(m_ptr);
	}
	cc*& id() {
		return m_id;
	}
private:
	cc* m_id{};
	u8* m_ptr{};
	std::vector<u8> m_bytes{};
	std::vector<u8> m_original{};
	bool m_set{};
};
class patches {
public:
	void add(cc* id, u8* ptr, std::vector<u8> bytes, bool apply = true) {
		m_patches.push_back(MakeSmartPointer<patch>(id, ptr, bytes));
		if (apply)
			m_patches.back()->apply();
	}
	template <typename t>
	void remove(cc* id) {
		if (patch* p = get<t>(id); p) {
			p->restore();
		}
	}
	patch* get(cc* id) {
		for (auto& p : m_patches) {
			if (p->id() == id) {
				return p.get();
			}
		}
		return nullptr;
	}
	void reset() {
		for (auto& p : m_patches) {
			p->restore();
		}
		m_patches.clear();
	}
private:
	std::vector<SmartPointer<patch>> m_patches{};
};
inline patches g_patches{};
class arxPatches {
public:
	void addInteg(u8* ptr) {
		const std::vector<u8> bytes{ 0x90ui8, 0x90ui8, 0x90ui8, 0x90ui8, 0x90ui8, 0x90ui8, 0x90ui8 };
		m_integCheckPatches.push_back(new patch(NULL, ptr, bytes, true));
		m_integCheckPatches.back()->apply();
	}
	void addCodeHealer(u8* ptr) {

	}
	void clear() {
		for (auto& p : m_integCheckPatches) {
			p->restore();
		}
		for (auto& p : m_codeHealerPatches) {
			p->restore();
		}
		m_integCheckPatches.clear();
		m_codeHealerPatches.clear();
	}
	size_t count() {
		return integCount() + codeHealCount();
	}
	size_t integCount() {
		return m_integCheckPatches.size();
	}
	size_t codeHealCount() {
		return m_codeHealerPatches.size();
	}
private:
	std::vector<patch*> m_integCheckPatches{};
	std::vector<patch*> m_codeHealerPatches{};
};
inline arxPatches g_arxPatches{};