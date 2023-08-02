#pragma once
#include "pch/pch.h"
#include "rage/classes.h"
#include "rage/joaat.h"
#include "collections.h"
#pragma pack(push, 1)
struct u24 {
	i32 i : 24;
};
#pragma pack(pop)
#define PUSH8(v) *(u8*)m_opcodesTbl = (u8)v; m_opcodesTbl++
#define PUSH16(v) *(u16*)m_opcodesTbl = (u16)v; m_opcodesTbl+=2
#define PUSH24(v) (*(u24*)m_opcodesTbl).i = (u32)v; m_opcodesTbl+=3
#define PUSH32(v) *(u32*)m_opcodesTbl = (u32)v; m_opcodesTbl+=4
#define OSTART(size) \
	constexpr int SIZE{ size}; \
	if (m_labelMode) { \
	} \
	else 
#define OEND ; m_ip += SIZE

namespace rage::ysc {
	class program {
	public:
		program(fnptr<void(program&)> callback) {
			u8* opcodes{ m_codePageCollection.getPage(2048) };
			m_labelMode = true;
			callback(*this);
			m_labelMode = false;
			m_ip = NULL;
			m_opcodesTbl = opcodes;
			callback(*this);
			m_codePageCollection.getCodePageSize() = (m_opcodesTbl - opcodes) + 1;
		}

		template <typename t>
		void push(t val) {
			OSTART(5) {
				PUSH8(OP_PUSH_CONST_U32);
				if constexpr (!std::is_same_v<t, uint32_t>) {
					PUSH32(*(uint32_t*)&val);
				}
				else {
					PUSH32(val);
				}
			} OEND;
		}
		template <typename t>
		void tPush(t value) {
			if constexpr (std::is_same_v<t, const char*>) {
				push_string(std::forward<t>(value));
			}
			else {
				push(std::forward<t>(value));
			}
		}
		void pushf(float val) {
			OSTART(5) {
				PUSH8(OP_PUSH_CONST_U32);
				PUSH32(*(uint32_t*)&val);
			} OEND;
		}
		void iadd() {
			OSTART(1) {
				PUSH8(OP_IADD);
			} OEND;
		}
		void isub() {
			OSTART(1) {
				PUSH8(OP_ISUB);
			} OEND;
		}
		void imul() {
			OSTART(1) {
				PUSH8(OP_IMUL);
			} OEND;
		}
		void idiv() {
			OSTART(1) {
				PUSH8(OP_IDIV);
			} OEND;
		}
		void fadd() {
			OSTART(1) {
				PUSH8(OP_FADD);
			} OEND;
		}
		void fsub() {
			OSTART(1) {
				PUSH8(OP_FSUB);
			} OEND;
		}
		void fmul() {
			OSTART(1) {
				PUSH8(OP_FMUL);
			} OEND;
		}
		void fdiv() {
			OSTART(1) {
				PUSH8(OP_FDIV);
			} OEND;
		}
		void vadd() {
			OSTART(1) {
				PUSH8(OP_VADD);
			} OEND;
		}
		void vsub() {
			OSTART(1) {
				PUSH8(OP_VSUB);
			} OEND;
		}
		void vmul() {
			OSTART(1) {
				PUSH8(OP_VMUL);
			} OEND;
		}
		void vdiv() {
			OSTART(1) {
				PUSH8(OP_VDIV);
			} OEND;
		}
		void ieq() {
			OSTART(1) {
				PUSH8(OP_IEQ);
			} OEND;
		}
		void inot() {
			OSTART(1) {
				PUSH8(OP_INOT);
			} OEND;
		}
		void ior() {
			OSTART(1) {
				PUSH8(OP_IOR);
			} OEND;
		}
		void iand() {
			OSTART(1) {
				PUSH8(OP_IAND);
			} OEND;
		}
		void label(const char* label) {
			if (m_labelMode) {
				m_labels[label] = m_ip;
			}
		}
		void enter(const char* name, uint8_t params, uint16_t locals) {
			if (m_labelMode) {
				m_labels[name] = m_ip;
			}
			OSTART(5) {
				PUSH8(OP_ENTER);
				PUSH8(params);
				PUSH16(locals);
				PUSH8(0); //funcNameLen
				m_currentParams = params;
			} OEND;
		}
		void leave(uint8_t params, uint8_t returns) {
			OSTART(3) {
				PUSH8(OP_LEAVE);
				PUSH8(params);
				PUSH8(returns);
			} OEND;
		}
		void drop() {
			OSTART(1) {
				PUSH8(OP_DROP);
			} OEND;
		}
		void dup() {
			OSTART(1) {
				PUSH8(OP_DUP);
			} OEND;
		}
		void f2v() {
			OSTART(1) {
				PUSH8(OP_F2V);
			} OEND;
		}
		void native(uint64_t hash, uint8_t params, uint8_t returns) {
			OSTART(4) {
				PUSH8(OP_NATIVE);
				PUSH8((params << 2) | (returns & 0x3));
				u32 nativeIndex{ getOrMakeNativeIndex(hash) };
				PUSH8((nativeIndex >> 8));
				PUSH8((nativeIndex & 0x00FF));
			} OEND;
		}
		void call(const char* label) {
			OSTART(4) {
				PUSH8(OP_CALL);
				PUSH24(getLabelInstructionPointer(label));
			} OEND;
		}
		void jmp(const char* label) {
			OSTART(3) {
				PUSH8(OP_J);
				u16 labelIp{ static_cast<u16>(getLabelInstructionPointer(label)) };
				PUSH16(labelIp - static_cast<u16>(m_ip) - 3);
			} OEND;
		}
		void jz(const char* label) {
			OSTART(3) {
				PUSH8(OP_JZ);
				u16 labelIp{ static_cast<u16>(getLabelInstructionPointer(label)) };
				PUSH16(labelIp - static_cast<u16>(m_ip) - 3);
			} OEND;
		}
		void je(const char* label) {
			OSTART(3) {
				PUSH8(OP_INE_JZ);
				u16 labelIp{ static_cast<u16>(getLabelInstructionPointer(label)) };
				PUSH16(labelIp - static_cast<u16>(m_ip) - 3);
			} OEND;
		}
		void jne(const char* label) {
			OSTART(3) {
				PUSH8(OP_IEQ_JZ);
				u16 labelIp{ static_cast<u16>(getLabelInstructionPointer(label)) };
				PUSH16(labelIp - static_cast<u16>(m_ip) - 3);
			} OEND;
		}
		void load() {
			OSTART(1) {
				PUSH8(OP_LOAD);
			} OEND;
		}
		void store() {
			OSTART(1) {
				PUSH8(OP_STORE);
			} OEND;
		}
		void global(uint32_t idx) {
			OSTART(4) {
				PUSH8(OP_GLOBAL_U24);
				PUSH24(idx);
			} OEND;
		}
		void global_load(uint32_t idx) {
			OSTART(4) {
				PUSH8(OP_GLOBAL_U24_LOAD);
				PUSH24(idx);
			} OEND;
		}
		void global_store(uint32_t idx) {
			OSTART(4) {
				PUSH8(OP_GLOBAL_U24_STORE);
				PUSH24(idx);
			} OEND;
		}
		void push_string(const char* string) {
			OSTART(6) {
				PUSH8(OP_PUSH_CONST_U32);
				PUSH32(getOrMakeStringIndex(string));
				PUSH8(OP_STRING);
			} OEND;
		}
		void local(uint16_t idx) {
			OSTART(3) {
				PUSH8(OP_STATIC_U16);
				PUSH16(idx);
			} OEND;
		}
		void local_load(uint16_t idx) {
			OSTART(3) {
				PUSH8(OP_STATIC_U16_LOAD);
				PUSH16(idx);
			} OEND;
		}
		void local_store(uint16_t idx) {
			OSTART(3) {
				PUSH8(OP_STATIC_U16_STORE);
				PUSH16(idx);
			} OEND;
		}
		void offset(uint16_t off) {
			OSTART(3) {
				PUSH8(OP_IOFFSET_S16);
				PUSH16(off);
			} OEND;
		}
		void offset_load(uint16_t off) {
			OSTART(3) {
				PUSH8(OP_IOFFSET_S16_LOAD);
				PUSH16(off);
			} OEND;
		}
		void offset_store(uint16_t off) {
			OSTART(3) {
				PUSH8(OP_IOFFSET_S16_STORE);
				PUSH16(off);
			} OEND;
		}
		void array(uint16_t size) {
			OSTART(3) {
				PUSH8(OP_ARRAY_U16);
				PUSH16(size);
			} OEND;
		}
		void array_load(uint16_t size) {
			OSTART(3) {
				PUSH8(OP_ARRAY_U16_LOAD);
				PUSH16(size);
			} OEND;
		}
		void array_store(uint16_t size) {
			OSTART(3) {
				PUSH8(OP_ARRAY_U16_STORE);
				PUSH16(size);
			} OEND;
		}
		void frame(uint16_t idx) {
			OSTART(3) {
				PUSH8(OP_LOCAL_U16);
				PUSH16(2 + m_currentParams + idx);
			} OEND;
		}
		void frame_load(uint16_t idx) {
			OSTART(3) {
				PUSH8(OP_LOCAL_U16_LOAD);
				PUSH16(2 + m_currentParams + idx);
			} OEND;
		}
		void frame_store(uint16_t idx) {
			OSTART(3) {
				PUSH8(OP_LOCAL_U16_STORE);
				PUSH16(2 + m_currentParams + idx);
			} OEND;
		}
	public:
		std::vector<u64> m_natives{};
		stringPageCollection m_stringPageCollection{};
		codePageCollection m_codePageCollection{};
	private:
		u32 m_currentParams{};
		std::map<ccp, u32> m_labels{};
		u32 m_ip{};
		u8* m_opcodesTbl{};
		bool m_labelMode{};
		u16 getOrMakeNativeIndex(u64 hash) {
			m_natives.emplace_back(hash);
			for (u64 i{}; i != m_natives.size(); i++) {
				if (m_natives[i] == hash) {
					return i;
				}
			}
			return NULL;
		}
		u32 getOrMakeStringIndex(const char* string) {
			return m_stringPageCollection.addString(string);
		}
		u32 getLabelInstructionPointer(const char* label) {
			if (!m_labels.count(label)) {
				throw "Cannot find label";
			}
			return m_labels[label];
		}
	};
}