#pragma once
#include "pch/pch.h"
#include "rage/classes.h"
#include "rage/joaat.h"
#include "collections.h"
#include "command_list.h"
#include "core/logger.h"
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

			m_codePageCollection.codePageSize = static_cast<u32>((m_opcodesTbl - opcodes) + 1ui32);
		}

		void nop() {
			OSTART(1)
				PUSH8(OP_NOP)
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
		void imod() {
			OSTART(1)
				PUSH8(OP_IMOD)
			OEND
		}
		void inot() {
			OSTART(1)
				PUSH8(OP_INOT)
			OEND
		}
		void ineg() {
			OSTART(1)
				PUSH8(OP_INEG)
			OEND
		}
		void ieq() {
			OSTART(1)
				PUSH8(OP_IEQ)
			OEND
		}
		void ine() {
			OSTART(1)
				PUSH8(OP_INE)
			OEND
		}
		void igt() {
			OSTART(1)
				PUSH8(OP_IGT)
			OEND
		}
		void ige() {
			OSTART(1)
				PUSH8(OP_IGE)
			OEND
		}
		void ilt() {
			OSTART(1)
				PUSH8(OP_ILT)
			OEND
		}
		void ile() {
			OSTART(1)
				PUSH8(OP_ILE)
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
		void fmod() {
			OSTART(1)
				PUSH8(OP_FMOD)
			OEND
		}
		void fneg() {
			OSTART(1)
				PUSH8(OP_FNEG)
			OEND
		}
		void feq() {
			OSTART(1)
				PUSH8(OP_FEQ)
			OEND
		}
		void fne() {
			OSTART(1)
				PUSH8(OP_FNE)
			OEND
		}
		void fgt() {
			OSTART(1)
				PUSH8(OP_FGT)
			OEND
		}
		void fge() {
			OSTART(1)
				PUSH8(OP_FGE)
			OEND
		}
		void flt() {
			OSTART(1)
				PUSH8(OP_FLT)
			OEND
		}
		void fle() {
			OSTART(1)
				PUSH8(OP_FLE)
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
		void vneg() {
			OSTART(1)
				PUSH8(OP_VNEG)
			OEND
		}
		void iand() {
			OSTART(1)
				PUSH8(OP_IAND)
			OEND
		}
		void ior() {
			OSTART(1)
				PUSH8(OP_IOR)
			OEND
		}
		void ixor() {
			OSTART(1)
				PUSH8(OP_IXOR)
			OEND
		}
		void i2f() {
			OSTART(1)
				PUSH8(OP_I2F)
			OEND
		}
		void f2i() {
			OSTART(1)
				PUSH8(OP_F2I)
			OEND
		}
		void f2v() {
			OSTART(1)
				PUSH8(OP_F2V)
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
		void dup() {
			OSTART(1)
				PUSH8(OP_DUP)
			OEND
		}
		void drop() {
			OSTART(1)
				PUSH8(OP_DROP)
			OEND
		}
		void native(u64 hash, u8 params, u8 returns) {
			OSTART(4)
				PUSH8(OP_NATIVE)
				PUSH8((params << 2) | (returns & 0x3))
				u32 nativeIdx {
				getOrMakeNativeIndex(hash)
			};
			PUSH8((nativeIdx >> 8));
			PUSH8((nativeIdx & 0x00FF));
			OEND
		}
		void label(std::string label) {
			if (m_labelMode) {
				addLabel(label);
			}
		}
		void enter(std::string name, u8 params, u16 locals) {
			if (!name.empty()) {
				label(name);
			}
			OSTART(5)
				PUSH8(OP_ENTER)
				PUSH8(params)
				PUSH16(locals)
				PUSH8(0) //funcNameLen
				m_currentParams = params;
			OEND
		}
		void function(u8 params, u16 locals) {
			enter({}, params, locals);
		}
		void leave(u8 params, u8 returns) {
			OSTART(3)
				PUSH8(OP_LEAVE)
				PUSH8(params)
				PUSH8(returns)
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
		void jmp(std::string label) {
			OSTART(3)
				PUSH8(OP_J)
				PUSH16(static_cast<u16>(getLabelInstructionPointer(label)) - static_cast<u16>(m_ip) - 3)
			OEND
		}
		void jz(std::string label) {
			OSTART(3)
				PUSH8(OP_JZ)
				PUSH16(static_cast<u16>(getLabelInstructionPointer(label)) - static_cast<u16>(m_ip) - 3)
			OEND
		}
		void je(std::string label) {
			OSTART(3)
				PUSH8(OP_INE_JZ)
				PUSH16(static_cast<u16>(getLabelInstructionPointer(label)) - static_cast<u16>(m_ip) - 3)
			OEND
		}
		void jne(std::string label) {
			OSTART(3)
				PUSH8(OP_IEQ_JZ)
				PUSH16(static_cast<u16>(getLabelInstructionPointer(label)) - static_cast<u16>(m_ip) - 3)
			OEND
		}
		void jgt(std::string label) {
			OSTART(3)
				PUSH8(OP_IGT_JZ)
				PUSH16(static_cast<u16>(getLabelInstructionPointer(label)) - static_cast<u16>(m_ip) - 3)
			OEND
		}
		void call(std::string label) {
			OSTART(4)
				PUSH8(OP_CALL)
				PUSH24(getLabelInstructionPointer(label))
			OEND
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
		void push_b2(u8 b1, u8 b2) {
			OSTART(5)
				PUSH8(OP_PUSH_CONST_U8_U8)
				PUSH8(b1)
				PUSH8(b2)
			OEND
		}
		void push_b3(u8 b1, u8 b2, u8 b3) {
			OSTART(5)
				PUSH8(OP_PUSH_CONST_U8_U8_U8)
				PUSH8(b1)
				PUSH8(b2)
				PUSH8(b3)
			OEND
		}
		void pushf(float val) {
			push<float>(val);
		}
		template <typename t>
		void tPush(t value) {
			/*if (m_ip >= 4 && m_opcodesTbl[m_ip - 4] == OP_NATIVE && m_opcodesTbl[m_ip - 3] != 0) {
				return;
			}*/
			if constexpr (std::is_same_v<t, cc*>) {
				push_string(std::forward<t>(value));
			}
			else if constexpr (std::is_same_v<t, float>) {
				pushf(std::forward<t>(value));
			}
			else {
				push<t>(std::forward<t>(value));
			}
		}
		void push_string(cc* string) {
			OSTART(6)
				PUSH8(OP_PUSH_CONST_U32)
				PUSH32(getOrMakeStringIndex(string))
				PUSH8(OP_STRING)
			OEND
		}
		void is_bit_set() {
			OSTART(1)
				PUSH8(OP_IS_BIT_SET)
			OEND
		}
		void push_hash_string(cc* string) {
			push<u32>(rage::joaat(string)); //It's much easier to just push the joaat directly
		}
	public:
		u32 m_staticCount{};
		u32 m_stackSize{};
		std::vector<u64> m_natives{};
		stringPageCollection m_stringPageCollection{};
		codePageCollection m_codePageCollection{};
	private:
		u32 m_currentParams{};
		std::map<std::string, u32> m_labels{};
		u32 m_ip{};
		u8* m_opcodesTbl{};
		bool m_labelMode{};
		u16 getOrMakeNativeIndex(u64 hash) {
			m_natives.emplace_back(hash);
			for (u16 i{}; i != static_cast<u16>(m_natives.size()); i++) {
				if (m_natives[i] == hash) {
					return i;
				}
			}
			return NULL;
		}
		u32 getOrMakeStringIndex(cc* string) {
			return m_stringPageCollection.addString(string);
		}
		u32 getLabelInstructionPointer(std::string label) {
			if (!m_labels.count(label)) {
				throw "Cannot find label";
			}
			return m_labels[label];
		}
		void addLabel(std::string label) {
			m_labels.insert({ label, m_ip });
		}
	};
}