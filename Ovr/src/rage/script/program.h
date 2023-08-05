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
#define PUSH8(t) *(u8*)m_opcodesTbl = (u8)t; m_opcodesTbl += 1;
#define PUSH16(t) *(u16*)m_opcodesTbl = (u16)t; m_opcodesTbl += 2;
#define PUSH32(t) *(u32*)m_opcodesTbl = (u32)t; m_opcodesTbl += 4;
#define PUSH24(t) (*(u24*)m_opcodesTbl).i = (u32)t; m_opcodesTbl += 3;
#define OSTART(size) constexpr int SIZE = size; if (m_labelMode) {}  else {
#define OEND }; m_ip += SIZE ;

namespace rage::ysc {
	class program {
	public:
		program(fnptr<void(program&)> callback) {
			uint8_t* opcodes{ m_codePageCollection.getPage(2048) };
			m_labelMode = true;
			callback(*this);
			m_labelMode = false;
			m_ip = 0;
			m_opcodesTbl = opcodes;
			callback(*this);

			m_codePageCollection.codePageSize = (m_opcodesTbl - opcodes) + 1;
		}

		template <typename t>
		void push(t val) {
			OSTART(5)
				PUSH8(OP_PUSH_CONST_U32)
				if constexpr (!std::is_same_v<t, u32>) {
					PUSH32(*(u32*)&val)
				}
				else {
					PUSH32(val)
				}
			OEND
		}
		template <typename t>
		void tPush(t value) {
			if constexpr (std::is_same_v<t, ccp>) {
				push_string(std::forward<t>(value));
			}
			else {
				push(std::forward<t>(value));
			}
		}
		void pushf(float val) {
			OSTART(5)
				PUSH8(OP_PUSH_CONST_U32)
				PUSH32(*(u32*)&val)
			OEND
		}
		void iadd() {
			OSTART(1)
				PUSH8(OP_IADD)
			OEND
		}
		void isub() {
			OSTART(1)
				PUSH8(OP_ISUB)
			OEND
		}
		void imul() {
			OSTART(1)
				PUSH8(OP_IMUL)
			OEND
		}
		void idiv() {
			OSTART(1)
				PUSH8(OP_IDIV)
			OEND
		}
		void fadd() {
			OSTART(1)
				PUSH8(OP_FADD)
			OEND
		}
		void fsub() {
			OSTART(1)
				PUSH8(OP_FSUB)
			OEND
		}
		void fmul() {
			OSTART(1)
				PUSH8(OP_FMUL)
			OEND
		}
		void fdiv() {
			OSTART(1)
				PUSH8(OP_FDIV)
			OEND
		}
		void vadd() {
			OSTART(1)
				PUSH8(OP_VADD)
			OEND
		}
		void vsub() {
			OSTART(1)
				PUSH8(OP_VSUB)
			OEND
		}
		void vmul() {
			OSTART(1)
				PUSH8(OP_VMUL)
			OEND
		}
		void vdiv() {
			OSTART(1)
				PUSH8(OP_VDIV)
			OEND
		}
		void ieq() {
			OSTART(1)
				PUSH8(OP_IEQ)
			OEND
		}
		void inot() {
			OSTART(1)
				PUSH8(OP_INOT)
			OEND
		}
		void ior() {
			OSTART(1)
				PUSH8(OP_IOR)
			OEND
		}
		void iand() {
			OSTART(1)
				PUSH8(OP_IAND)
			OEND
		}
		void label(ccp label) {
			if (m_labelMode)
				m_labels[label] = m_ip;
		}
		void enter(ccp name, u8 params, u16 locals) {
			label(name);
			OSTART(5)
				PUSH8(OP_ENTER)
				PUSH8(params)
				PUSH16(locals)
				PUSH8(0) //funcNameLen
				m_currentParams = params;
			OEND
		}
		void leave(u8 params, u8 returns) {
			OSTART(3)
				PUSH8(OP_LEAVE)
				PUSH8(params)
				PUSH8(returns)
			OEND
		}
		void drop() {
			OSTART(1)
				PUSH8(OP_DROP)
			OEND
		}
		void dup() {
			OSTART(1)
				PUSH8(OP_DUP)
			OEND
		}
		void f2v() {
			OSTART(1)
				PUSH8(OP_F2V)
			OEND
		}
		void native(u64 hash, u8 params, u8 returns) {
			OSTART(4)
				PUSH8(OP_NATIVE)
				PUSH8((params << 2) | (returns & 0x3))
				u32 nativeIdx{ getOrMakeNativeIndex(hash) };
				PUSH8((nativeIdx >> 8));
				PUSH8((nativeIdx & 0x00FF));
			OEND
		}
		void call(ccp label) {
			OSTART(4)
				PUSH8(OP_CALL)
				PUSH24(getLabelInstructionPointer(label))
			OEND
		}
		void jmp(ccp label) {
			OSTART(3)
				PUSH8(OP_J)
				PUSH16(static_cast<u16>(getLabelInstructionPointer(label)) - static_cast<u16>(m_ip) - 3)
			OEND
		}
		void jz(ccp label) {
			OSTART(3)
				PUSH8(OP_JZ)
				PUSH16(static_cast<u16>(getLabelInstructionPointer(label)) - static_cast<u16>(m_ip) - 3)
			OEND
		}
		void je(ccp label) {
			OSTART(3)
				PUSH8(OP_INE_JZ)
				PUSH16(static_cast<u16>(getLabelInstructionPointer(label)) - static_cast<u16>(m_ip) - 3)
			OEND
		}
		void jne(ccp label) {
			OSTART(3)
				PUSH8(OP_IEQ_JZ)
				PUSH16(static_cast<u16>(getLabelInstructionPointer(label)) - static_cast<u16>(m_ip) - 3)
			OEND
		}
		void load() {
			OSTART(1)
				PUSH8(OP_LOAD)
			OEND
		}
		void store() {
			OSTART(1)
				PUSH8(OP_STORE)
			OEND
		}
		void global(u32 idx) {
			OSTART(4)
				PUSH8(OP_GLOBAL_U24)
				PUSH24(idx)
			OEND
		}
		void global_load(u32 idx) {
			OSTART(4)
				PUSH8(OP_GLOBAL_U24_LOAD)
				PUSH24(idx)
			OEND
		}
		void global_store(u32 idx) {
			OSTART(4)
				PUSH8(OP_GLOBAL_U24_STORE)
				PUSH24(idx)
			OEND
		}
		void push_string(ccp string) {
			OSTART(6)
				PUSH8(OP_PUSH_CONST_U32)
				PUSH32(getOrMakeStringIndex(string))
				PUSH8(OP_STRING)
			OEND
		}
		void local(u16 idx) {
			OSTART(3)
				PUSH8(OP_STATIC_U16)
				PUSH16(idx)
			OEND
		}
		void local_load(u16 idx) {
			OSTART(3)
				PUSH8(OP_STATIC_U16_LOAD)
				PUSH16(idx)
			OEND
		}
		void local_store(u16 idx) {
			OSTART(3)
				PUSH8(OP_STATIC_U16_STORE)
				PUSH16(idx)
			OEND
		}
		void offset(u16 off) {
			OSTART(3)
				PUSH8(OP_IOFFSET_S16)
				PUSH16(off)
			OEND
		}
		void offset_load(u16 off) {
			OSTART(3)
				PUSH8(OP_IOFFSET_S16_LOAD)
				PUSH16(off)
			OEND
		}
		void offset_store(u16 off) {
			OSTART(3)
				PUSH8(OP_IOFFSET_S16_STORE)
				PUSH16(off)
			OEND
		}
		void array(u16 size) {
			OSTART(3)
				PUSH8(OP_ARRAY_U16)
				PUSH16(size)
			OEND
		}
		void array_load(u16 size) {
			OSTART(3)
				PUSH8(OP_ARRAY_U16_LOAD)
				PUSH16(size)
			OEND
		}
		void array_store(u16 size) {
			OSTART(3)
				PUSH8(OP_ARRAY_U16_STORE)
				PUSH16(size)
			OEND
		}
		void frame(u16 idx) {
			OSTART(3)
				PUSH8(OP_LOCAL_U16)
				PUSH16(2 + m_currentParams + idx)
			OEND
		}
		void frame_load(u16 idx) {
			OSTART(3)
				PUSH8(OP_LOCAL_U16_LOAD)
				PUSH16(2 + m_currentParams + idx)
			OEND
		}
		void frame_store(u16 idx) {
			OSTART(3)
				PUSH8(OP_LOCAL_U16_STORE)
				PUSH16(2 + m_currentParams + idx)
			OEND
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
		u32 getOrMakeStringIndex(ccp string) {
			return m_stringPageCollection.addString(string);
		}
		u32 getLabelInstructionPointer(ccp label) {
			if (!m_labels.count(label)) {
				throw "Cannot find label";
			}
			return m_labels[label];
		}
	};
}