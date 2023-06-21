#include "hooking/hooking.h"
#include "hooking/methods/native.h"
#include "commands/manager/manager.h"
#define CASE(c) case c:
#define S_CASE(c) c:
#define FETCH_INSN {
#define NEXT_INSN break; }
#define SET_PC(_o) ONCE({ i64 o{ _o }; pc = (opcodesTbl[o >> 14] + (o & 0x3FFF) - 1); opcodes = pc - o; });
#define SET_STATE_AND_RET(state) ser->m_state = state; return Serialised->m_state;
#define FAULT(s, ...) LOG(Fatal, s, __VA_ARGS__); continue;
#define LoadImm8 (*++pc)
#define LoadImm16 ((pc+=2), *(uint16_t*)(pc-1))
#define LoadImmS16 ((pc+=2), *(int16_t*)(pc-1))
#define LoadImm24 ((pc+=3), *(uint32_t*)(pc-3) >> 8)
#define LoadImm32 ((pc+=4), *(uint32_t*)(pc-3))

void scrItoa(char* dest, int value) {
	char stack[16]{}, * sp = stack;
	if (value < 0) {
		*dest++ = '-';
		value = -value;
	}
	else if (!value) {
		dest[0] = '0';
		dest[1] = 0;
		return;
	}
	while (value) {
		*sp++ = (char)((value % 10) + '0');
		value /= 10;
	}
	while (sp != stack)
		*dest++ = *--sp;
	*dest = 0;
}
void scrAssignString(char* dst, unsigned size, const char* src) {
	if (src) {
		while (*src && --size)
			*dst++ = *src++;
	}
	*dst = '\0';
}
void scrAppendString(char* dst, unsigned size, const char* src) {
	while (*dst)
		dst++, --size;
	scrAssignString(dst, size, src);
}

rage::eThreadState hooks::scriptVm(rage::scrValue* Stack, rage::scrValue** Globals, rage::scrProgram* Program, rage::scrThreadSerialised* Serialised) {
	u8** opcodesTbl{ Program->m_code_blocks };
	rage::scrValue* sp{ Stack + Serialised->m_stack_pointer - 1 };
	rage::scrValue* fp{ Stack + Serialised->m_frame_pointer };
	u8* pc{};
	u8* opcodes{};
	SET_PC(Serialised->m_pointer_count);
	CHAR buf[0x10]{};
	static rage::scrValue dummy{};
	while (true) {
		switch (LoadImm8) {
			CASE(OP_NOP) FETCH_INSN; SET_PC(pc - opcodes); NEXT_INSN;
			CASE(OP_IADD) FETCH_INSN; --sp; sp[0].Int += sp[1].Int; NEXT_INSN;
			CASE(OP_ISUB) FETCH_INSN; --sp; sp[0].Int -= sp[1].Int; NEXT_INSN;
			CASE(OP_IMUL) FETCH_INSN; --sp; sp[0].Int *= sp[1].Int; NEXT_INSN;
			CASE(OP_IDIV) FETCH_INSN; --sp; if (sp[1].Int) sp[0].Int /= sp[1].Int; NEXT_INSN;
			CASE(OP_IMOD) FETCH_INSN; --sp; if (sp[1].Int) sp[0].Int %= sp[1].Int; NEXT_INSN;
			CASE(OP_INOT) FETCH_INSN; sp[0].Int = !sp[0].Int; NEXT_INSN;
			CASE(OP_INEG) FETCH_INSN; sp[0].Int = -sp[0].Int; NEXT_INSN;

			CASE(OP_IEQ) FETCH_INSN; --sp; sp[0].Int = sp[0].Int == sp[1].Int; NEXT_INSN;
			CASE(OP_INE) FETCH_INSN; --sp; sp[0].Int = sp[0].Int != sp[1].Int; NEXT_INSN;
			CASE(OP_IGE) FETCH_INSN; --sp; sp[0].Int = sp[0].Int >= sp[1].Int; NEXT_INSN;
			CASE(OP_IGT) FETCH_INSN; --sp; sp[0].Int = sp[0].Int > sp[1].Int; NEXT_INSN;
			CASE(OP_ILE) FETCH_INSN; --sp; sp[0].Int = sp[0].Int <= sp[1].Int; NEXT_INSN;
			CASE(OP_ILT) FETCH_INSN; --sp; sp[0].Int = sp[0].Int < sp[1].Int; NEXT_INSN;

			CASE(OP_FADD) FETCH_INSN; --sp; sp[0].Float += sp[1].Float; NEXT_INSN;
			CASE(OP_FSUB) FETCH_INSN; --sp; sp[0].Float -= sp[1].Float; NEXT_INSN;
			CASE(OP_FMUL) FETCH_INSN; --sp; sp[0].Float *= sp[1].Float; NEXT_INSN;
			CASE(OP_FDIV) FETCH_INSN; --sp; if (sp[1].Int) sp[0].Float /= sp[1].Float; NEXT_INSN;
			CASE(OP_FMOD) FETCH_INSN; --sp; if (sp[1].Int) sp[0].Float = (sp[1].Float ? sp[0].Float - ((i32)(sp[0].Float / sp[1].Float) * sp[1].Float) : 0); NEXT_INSN;
			CASE(OP_FNEG) FETCH_INSN; sp[0].Uns ^= 0x80000000; NEXT_INSN;

			CASE(OP_FEQ) FETCH_INSN; --sp; sp[0].Int = sp[0].Float == sp[1].Float; NEXT_INSN;
			CASE(OP_FNE) FETCH_INSN; --sp; sp[0].Int = sp[0].Float != sp[1].Float; NEXT_INSN;
			CASE(OP_FGE) FETCH_INSN; --sp; sp[0].Int = sp[0].Float >= sp[1].Float; NEXT_INSN;
			CASE(OP_FGT) FETCH_INSN; --sp; sp[0].Int = sp[0].Float > sp[1].Float; NEXT_INSN;
			CASE(OP_FLE) FETCH_INSN; --sp; sp[0].Int = sp[0].Float <= sp[1].Float; NEXT_INSN;
			CASE(OP_FLT) FETCH_INSN; --sp; sp[0].Int = sp[0].Float < sp[1].Float; NEXT_INSN;

			CASE(OP_VADD) FETCH_INSN; sp -= 3; sp[-2].Float += sp[1].Float; sp[-1].Float += sp[2].Float; sp[0].Float += sp[3].Float; NEXT_INSN;
			CASE(OP_VSUB) FETCH_INSN; sp -= 3; sp[-2].Float -= sp[1].Float; sp[-1].Float -= sp[2].Float; sp[0].Float -= sp[3].Float; NEXT_INSN;
			CASE(OP_VMUL) FETCH_INSN; sp -= 3; sp[-2].Float *= sp[1].Float; sp[-1].Float *= sp[2].Float; sp[0].Float *= sp[3].Float; NEXT_INSN;
			CASE(OP_VDIV) FETCH_INSN; sp -= 3; if (sp[1].Int) sp[-2].Float /= sp[1].Float; if (sp[2].Int) sp[-1].Float /= sp[2].Float; if (sp[3].Int) sp[0].Float /= sp[3].Float; NEXT_INSN;
			CASE(OP_VNEG) FETCH_INSN; sp[-2].Uns ^= 0x80000000; sp[-1].Uns ^= 0x80000000; sp[0].Uns ^= 0x80000000; NEXT_INSN;

			CASE(OP_IAND) FETCH_INSN; --sp; sp[0].Int &= sp[1].Int; NEXT_INSN;
			CASE(OP_IOR)  FETCH_INSN; --sp; sp[0].Int |= sp[1].Int; NEXT_INSN;
			CASE(OP_IXOR) FETCH_INSN; --sp; sp[0].Int ^= sp[1].Int; NEXT_INSN;

			CASE(OP_I2F) FETCH_INSN; sp[0].Float = (float_t)sp[0].Int; NEXT_INSN;
			CASE(OP_F2I) FETCH_INSN; sp[0].Int = (i32)sp[0].Float; NEXT_INSN;
			CASE(OP_F2V) FETCH_INSN; sp += 2; sp[-1].Int = sp[0].Int = sp[-2].Int; NEXT_INSN;

			CASE(OP_PUSH_CONST_U8) FETCH_INSN; ++sp; sp[0].Int = LoadImm8; NEXT_INSN;
			CASE(OP_PUSH_CONST_U8_U8) FETCH_INSN; sp += 2; sp[-1].Int = LoadImm8; sp[0].Int = LoadImm8; NEXT_INSN;
			CASE(OP_PUSH_CONST_U8_U8_U8) FETCH_INSN; sp += 3; sp[-2].Int = LoadImm8; sp[-1].Int = LoadImm8; sp[0].Int = LoadImm8; NEXT_INSN;

			CASE(OP_PUSH_CONST_U32) FETCH_INSN; ++sp; sp[0].Uns = LoadImm32; NEXT_INSN;
			CASE(OP_PUSH_CONST_F) FETCH_INSN; ++sp; sp[0].Uns = LoadImm32; NEXT_INSN;

			CASE(OP_DUP) FETCH_INSN; ++sp; sp[0].Any = sp[-1].Any; NEXT_INSN;
			CASE(OP_DROP) FETCH_INSN; --sp; NEXT_INSN;

			CASE(OP_NATIVE) FETCH_INSN;
				u64 returnSize{ LoadImm8 };
				u64 paramCount{ (returnSize >> 2) & 63 };
				i32 imm{ (LoadImm8 << 8) };
				imm |= LoadImm8;
				returnSize &= 3;
				rage::scrCmd cmd{ (decltype(cmd))Program->m_natives[imm] };
				Serialised->m_pointer_count = (i32)(pc - opcodes - 4);
				Serialised->m_frame_pointer = (i32)(fp - Stack);
				Serialised->m_stack_pointer = (i32)(sp - Stack + 1);
				rage::scrNativeCallContext curInfo(returnSize ? &Stack[Serialised->m_stack_pointer - paramCount] : 0, paramCount, &Stack[Serialised->m_stack_pointer - paramCount]);
				if (cmd == pointers::g_nativeRegistrationTable->get_handler(0xEE8559BBFC27701B)) {
					Entity entity{ curInfo.getArg<Entity>(0) };
					const char* propertyName{ curInfo.getArg<const char*>(1) };
					int value{ curInfo.getArg<int>(2) };
					DECORATOR::DECOR_SET_INT(entity, propertyName, value);
					printf("call for DSI\n");
				}
				else {
					(*cmd)(&curInfo);
				}
				if (Serialised->m_state != rage::eThreadState::running)
					return Serialised->m_state;
				curInfo.VectorSpace.CopyReferencedParametersOut();
				sp -= paramCount - returnSize;
			NEXT_INSN;

			CASE(OP_ENTER) FETCH_INSN;
				u32 paramCount{ LoadImm8 };
				u32 localCount{ LoadImm16 };
				u32 nameCount{ LoadImm8 };
				if (Serialised->m_call_depth < 16)
					Serialised->m_callstack[Serialised->m_call_depth] = (int32_t)(pc - opcodes + 2);
				++(Serialised->m_call_depth);
				pc += nameCount;
				if (sp - Stack >= (i32)(Serialised->m_stack_size - localCount)) {
					LoadImm8;
					FAULT("Stack overflow");
				}
				(++sp)->Int = (int32_t)(fp - Stack);
				fp = sp - paramCount - 1;
				while (localCount--)
					(++sp)->Any = 0;
				sp -= paramCount;
			NEXT_INSN;

			CASE(OP_LEAVE) FETCH_INSN;
				--(Serialised->m_call_depth);
				u32 paramCount{ LoadImm8 };
				u32 returnSize{ LoadImm8 };
				rage::scrValue* result{ sp - returnSize };
				sp = fp + paramCount - 1;
				fp = Stack + sp[2].Uns;
				uint32_t newPc = sp[1].Uns;
				SET_PC(newPc);
				sp -= paramCount;
				while (returnSize--)
					*++sp = *++result;
				if (!newPc)
					return Serialised->m_state = rage::eThreadState::killed;
			NEXT_INSN;

			CASE(OP_LOAD) FETCH_INSN; sp[0].Any = (sp[0].Reference)->Any; NEXT_INSN;
			CASE(OP_STORE) FETCH_INSN; sp -= 2; (sp[2].Reference)->Any = sp[1].Any; NEXT_INSN;
			CASE(OP_STORE_REV) FETCH_INSN; --sp; (sp[0].Reference)->Any = sp[1].Any; NEXT_INSN;

			CASE(OP_LOAD_N) FETCH_INSN;
				rage::scrValue* addr{ (sp--)->Reference };
				i32 count{ (sp--)->Int };
				for (u32 i{}; i != count; ++i)
					(++sp)->Any = addr[i].Any;
			NEXT_INSN;

			CASE(OP_STORE_N) FETCH_INSN;
				rage::scrValue* addr{ (sp--)->Reference };
				i32 count{ (sp--)->Int };
				for (u32 i{}; i != count; ++i)
					addr[count - 1 - i].Any = (sp--)->Any;
			NEXT_INSN;

			CASE(OP_ARRAY_U8) FETCH_INSN;
				--sp;
				rage::scrValue* r{ sp[1].Reference };
				u64 idx{ sp[0].Uns };
				if (idx >= r->Uns) {
					switch ("arrayOverrunKickProtection"_PF->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "Ovverun kick from {}", g_lastScriptEventSender->GetName());
					} break;
					case eProtectionState::Block: {
						idx = 0;
					} break;
					case eProtectionState::BlockAndNotify: {
						idx = 0;
						LOG(Session, "Ovverun kick from {}", g_lastScriptEventSender->GetName());
					} break;
					}
					if ("arrayOverrunKickProtection"_PF->state() < eProtectionState::Block) {
						LoadImm8;
						sp[0].Reference = &dummy;
						FAULT("Array overrun, {} >= {}", idx, r->Uns);
					}
				}
				r += 1U + idx * LoadImm8;
				sp[0].Reference = (r);
			NEXT_INSN;

			CASE(OP_ARRAY_U8_LOAD) FETCH_INSN;
				--sp;
				rage::scrValue* r{ sp[1].Reference };
				size_t idx{ sp[0].Uns };
				if (idx >= r->Uns) {
					switch ("arrayOverrunKickProtection"_PF->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "Ovverun kick from {}", g_lastScriptEventSender->GetName());
					} break;
					case eProtectionState::Block: {
						idx = 0;
					} break;
					case eProtectionState::BlockAndNotify: {
						idx = 0;
						LOG(Session, "Ovverun kick from {}", g_lastScriptEventSender->GetName());
					} break;
					}
					if ("arrayOverrunKickProtection"_PF->state() < eProtectionState::Block) {
						LoadImm8;
						FAULT("Array overrun, {} >= {}", idx, r->Uns);
					}
				}
				r += 1U + idx * LoadImm8;
				sp[0].Any = r->Any;
			NEXT_INSN;

			CASE(OP_ARRAY_U8_STORE) FETCH_INSN;
				sp -= 3;
				rage::scrValue* r{ sp[3].Reference };
				size_t idx{ sp[2].Uns };
				if (idx >= r->Uns) {
					switch ("arrayOverrunKickProtection"_PF->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "Ovverun kick from {}", g_lastScriptEventSender->GetName());
					} break;
					case eProtectionState::Block: {
						idx = 0;
					} break;
					case eProtectionState::BlockAndNotify: {
						idx = 0;
						LOG(Session, "Ovverun kick from {}", g_lastScriptEventSender->GetName());
					} break;
					}
					if ("arrayOverrunKickProtection"_PF->state() < eProtectionState::Block) {
						LoadImm8;
						FAULT("Array overrun, {} >= {}", idx, r->Uns);
					}
				}
				r += 1U + idx * LoadImm8;
				r->Any = sp[1].Any;
			NEXT_INSN;

			CASE(OP_LOCAL_U8) FETCH_INSN; ++sp; sp[0].Reference = (fp + LoadImm8); NEXT_INSN;
			CASE(OP_LOCAL_U8_LOAD) FETCH_INSN; ++sp; sp[0].Any = fp[LoadImm8].Any; NEXT_INSN;
			CASE(OP_LOCAL_U8_STORE) FETCH_INSN; --sp; fp[LoadImm8].Any = sp[1].Any; NEXT_INSN;

			CASE(OP_STATIC_U8) FETCH_INSN; ++sp; sp[0].Reference = Stack + LoadImm8; NEXT_INSN;
			CASE(OP_STATIC_U8_LOAD) FETCH_INSN; ++sp; sp[0].Any = Stack[LoadImm8].Any; NEXT_INSN;
			CASE(OP_STATIC_U8_STORE) FETCH_INSN; --sp; Stack[LoadImm8].Any = sp[1].Any; NEXT_INSN;

			CASE(OP_IADD_U8) FETCH_INSN; sp[0].Int += LoadImm8; NEXT_INSN;
			CASE(OP_IMUL_U8) FETCH_INSN; sp[0].Int *= LoadImm8; NEXT_INSN;

			CASE(OP_IOFFSET) FETCH_INSN; --sp; sp[0].Any += sp[1].Int * sizeof(rage::scrValue); NEXT_INSN;
			CASE(OP_IOFFSET_U8) FETCH_INSN; sp[0].Any += LoadImm8 * sizeof(rage::scrValue); NEXT_INSN;
			CASE(OP_IOFFSET_U8_LOAD) FETCH_INSN; sp[0].Any = sp[0].Reference[LoadImm8].Any; NEXT_INSN;
			CASE(OP_IOFFSET_U8_STORE) FETCH_INSN; sp -= 2; sp[2].Reference[LoadImm8].Any = sp[1].Any; NEXT_INSN;

			CASE(OP_PUSH_CONST_S16) FETCH_INSN; ++sp; sp[0].Int = LoadImmS16; NEXT_INSN;
			CASE(OP_IADD_S16) FETCH_INSN; sp[0].Int += LoadImmS16; NEXT_INSN;
			CASE(OP_IMUL_S16) FETCH_INSN; sp[0].Int *= LoadImmS16; NEXT_INSN;

			CASE(OP_IOFFSET_S16) FETCH_INSN; sp[0].Any += LoadImmS16 * sizeof(rage::scrValue); NEXT_INSN;
			CASE(OP_IOFFSET_S16_LOAD) FETCH_INSN; sp[0].Any = sp[0].Reference[LoadImmS16].Any; NEXT_INSN;
			CASE(OP_IOFFSET_S16_STORE) FETCH_INSN; sp -= 2; sp[2].Reference[LoadImmS16].Any = sp[1].Any; NEXT_INSN;

			CASE(OP_ARRAY_U16) FETCH_INSN;
				--sp;
				rage::scrValue* r{ sp[1].Reference };
				size_t idx{ sp[0].Uns };
				if (idx >= r->Uns) {
					switch ("arrayOverrunKickProtection"_PF->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "Ovverun kick from {}", g_lastScriptEventSender->GetName());
					} break;
					case eProtectionState::Block: {
						idx = 0;
					} break;
					case eProtectionState::BlockAndNotify: {
						idx = 0;
						LOG(Session, "Ovverun kick from {}", g_lastScriptEventSender->GetName());
					} break;
					}
					if ("arrayOverrunKickProtection"_PF->state() < eProtectionState::Block) {
						LoadImm8;
						sp[0].Reference = &dummy;
						FAULT("Array overrun, {} >= {}", idx, r->Uns);
					}
				}
				r += 1U + idx * LoadImm16;
				sp[0].Reference = r;
			NEXT_INSN;

			CASE(OP_ARRAY_U16_LOAD) FETCH_INSN;
				--sp;
				rage::scrValue* r{ sp[1].Reference };
				size_t idx{ sp[0].Uns };
				if (idx >= r->Uns) {
					switch ("arrayOverrunKickProtection"_PF->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "Ovverun kick from {}", g_lastScriptEventSender->GetName());
					} break;
					case eProtectionState::Block: {
						idx = 0;
					} break;
					case eProtectionState::BlockAndNotify: {
						idx = 0;
						LOG(Session, "Ovverun kick from {}", g_lastScriptEventSender->GetName());
					} break;
					}
					if ("arrayOverrunKickProtection"_PF->state() < eProtectionState::Block) {
						LoadImm8;
						FAULT("Array overrun, {} >= {}", idx, r->Uns);
					}
				}
				r += 1U + idx * LoadImm16;
				sp[0].Any = r->Any;
			NEXT_INSN;

			CASE(OP_ARRAY_U16_STORE) FETCH_INSN;
				sp -= 3;
				rage::scrValue* r{ sp[3].Reference };
				size_t idx{ sp[2].Uns };
				if (idx >= r->Uns) {
					switch ("arrayOverrunKickProtection"_PF->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "Ovverun kick from {}", g_lastScriptEventSender->GetName());
					} break;
					case eProtectionState::Block: {
						idx = 0;
					} break;
					case eProtectionState::BlockAndNotify: {
						idx = 0;
						LOG(Session, "Ovverun kick from {}", g_lastScriptEventSender->GetName());
					} break;
					}
					if ("arrayOverrunKickProtection"_PF->state() < eProtectionState::Block) {
						LoadImm8;
						FAULT("Array overrun, {} >= {}", idx, r->Uns);
					}
				}
				r += 1U + idx * LoadImm16;
				r->Any = sp[1].Any;
			NEXT_INSN;

			CASE(OP_LOCAL_U16) FETCH_INSN; ++sp; sp[0].Reference = (fp + LoadImm16); NEXT_INSN;
			CASE(OP_LOCAL_U16_LOAD) FETCH_INSN; ++sp; sp[0].Any = fp[LoadImm16].Any; NEXT_INSN;
			CASE(OP_LOCAL_U16_STORE) FETCH_INSN; --sp; fp[LoadImm16].Any = sp[1].Any; NEXT_INSN;

			CASE(OP_STATIC_U16) FETCH_INSN; ++sp; sp[0].Reference = (Stack + LoadImm16); NEXT_INSN;
			CASE(OP_STATIC_U16_LOAD) FETCH_INSN; ++sp; sp[0].Any = Stack[LoadImm16].Any; NEXT_INSN;
			CASE(OP_STATIC_U16_STORE) FETCH_INSN; --sp; Stack[LoadImm16].Any = sp[1].Any; NEXT_INSN;

			CASE(OP_GLOBAL_U16) FETCH_INSN; ++sp; sp[0].Reference = Globals[0] + LoadImm16; NEXT_INSN;
			CASE(OP_GLOBAL_U16_LOAD) FETCH_INSN; ++sp; sp[0].Any = Globals[0][LoadImm16].Any; NEXT_INSN;
			CASE(OP_GLOBAL_U16_STORE) FETCH_INSN; --sp; Globals[0][LoadImm16].Any = sp[1].Any; NEXT_INSN;

			CASE(OP_STATIC_U24) FETCH_INSN; ++sp; sp[0].Reference = Stack + LoadImm24; NEXT_INSN;
			CASE(OP_STATIC_U24_LOAD) FETCH_INSN; ++sp; sp[0].Any = Stack[LoadImm24].Any; NEXT_INSN;
			CASE(OP_STATIC_U24_STORE) FETCH_INSN; --sp; Stack[LoadImm24].Any = sp[1].Any; NEXT_INSN;

			CASE(OP_GLOBAL_U24) FETCH_INSN;
				u32 imm{ LoadImm24 };
				u32 block{ imm >> 18 };
				u32 ofs{ imm & 0x3FFFF };
				++sp;
				if (!Globals[block]) {
					FAULT("Global block {}/{} not resident", block, ofs);
				}
				else {
					sp[0].Reference = (&Globals[block][ofs]);
				}
			NEXT_INSN;

			CASE(OP_GLOBAL_U24_LOAD) FETCH_INSN;
				u32 imm{ LoadImm24 };
				u32 block{ imm >> 18 };
				u32 ofs{ imm & 0x3FFFF };
				++sp;
				if (!Globals[block]) {
					FAULT("Global block {}/{} not resident", block, ofs);
				}
				else {
					sp[0].Any = Globals[block][ofs].Any;
				}
			NEXT_INSN;

			CASE(OP_GLOBAL_U24_STORE) FETCH_INSN;
				u32 imm{ LoadImm24 };
				u32 block{ imm >> 18 };
				u32 ofs{ imm & 0x3FFFF };
				--sp;
				if (!Globals[block]) {
					FAULT("Global block {}/{} not resident", block, ofs);
				}
				else {
					Globals[block][ofs].Any = sp[1].Any;
				}
			NEXT_INSN;

			CASE(OP_PUSH_CONST_U24) FETCH_INSN; ++sp; sp[0].Int = LoadImm24; NEXT_INSN;

			CASE(OP_CALL) FETCH_INSN; uint32_t imm = LoadImm24; ++sp; sp[0].Uns = (int32_t)(pc - opcodes); SET_PC(imm); NEXT_INSN;

			CASE(OP_J) FETCH_INSN; int32_t imm = LoadImmS16; SET_PC((pc - opcodes) + imm); NEXT_INSN;
			CASE(OP_JZ) FETCH_INSN; int32_t imm = LoadImmS16; --sp; if (sp[1].Int == 0) { SET_PC((pc - opcodes) + imm);} else { SET_PC(pc - opcodes); } NEXT_INSN;
			CASE(OP_IEQ_JZ) FETCH_INSN; int32_t imm = LoadImmS16; sp -= 2; if (!(sp[1].Int == sp[2].Int)) { SET_PC((pc - opcodes) + imm); } else { SET_PC(pc - opcodes); } NEXT_INSN;
			CASE(OP_INE_JZ) FETCH_INSN; int32_t imm = LoadImmS16; sp -= 2; if (!(sp[1].Int != sp[2].Int)) { SET_PC((pc - opcodes) + imm); } else { SET_PC(pc - opcodes); } NEXT_INSN;
			CASE(OP_IGE_JZ) FETCH_INSN; int32_t imm = LoadImmS16; sp -= 2; if (!(sp[1].Int >= sp[2].Int)) { SET_PC((pc - opcodes) + imm); } else { SET_PC(pc - opcodes); } NEXT_INSN;
			CASE(OP_IGT_JZ) FETCH_INSN; int32_t imm = LoadImmS16; sp -= 2; if (!(sp[1].Int > sp[2].Int)) { SET_PC((pc - opcodes) + imm); } else { SET_PC(pc - opcodes); } NEXT_INSN;
			CASE(OP_ILE_JZ) FETCH_INSN; int32_t imm = LoadImmS16; sp -= 2; if (!(sp[1].Int <= sp[2].Int)) { SET_PC((pc - opcodes) + imm); } else { SET_PC(pc - opcodes); } NEXT_INSN;
			CASE(OP_ILT_JZ) FETCH_INSN; int32_t imm = LoadImmS16; sp -= 2; if (!(sp[1].Int < sp[2].Int)) { SET_PC((pc - opcodes) + imm); } else { SET_PC(pc - opcodes); } NEXT_INSN;

			CASE(OP_SWITCH) FETCH_INSN;
				--sp;
				u32 label{ sp[1].Uns };
				u32 count{ LoadImm8 };
				SET_PC(pc - opcodes);
				for (u32 i{}; i != count; ++i) {
					u32 match{ LoadImm32 };
					u32 target{ LoadImm16 };
					if (label == match) {
						SET_PC((pc - opcodes) + target);
						break;
					}
				}
				SET_PC(pc - opcodes);
			NEXT_INSN;

			CASE(OP_STRING) FETCH_INSN;
				u32 offset{ sp[0].Uns };
				sp[0].String = (Program->m_strings_data[offset >> 14] + (offset & 0x3FFF));
			NEXT_INSN;

			CASE(OP_TEXT_LABEL_ASSIGN_STRING) FETCH_INSN;
				sp -= 2;
				LPSTR dest{ const_cast<LPSTR>(sp[2].String) };
				LPCSTR src{ (sp[1].String) };
				scrAssignString(dest, LoadImm8, src);
			NEXT_INSN;

			CASE(OP_TEXT_LABEL_ASSIGN_INT) FETCH_INSN;
				sp -= 2;
				LPSTR dest{ const_cast<LPSTR>(sp[2].String) };
				i32 value{ sp[1].Int };
				scrItoa(buf, value);
				scrAssignString(dest, LoadImm8, buf);
			NEXT_INSN;

			CASE(OP_TEXT_LABEL_APPEND_STRING) FETCH_INSN;
				sp -= 2;
				LPSTR dest{ const_cast<LPSTR>(sp[2].String) };
				LPCSTR src{ (sp[1].String) };
				scrAppendString(dest, LoadImm8, src);
			NEXT_INSN;

			CASE(OP_TEXT_LABEL_APPEND_INT) FETCH_INSN;
				sp -= 2;
				LPSTR dest{ const_cast<LPSTR>(sp[2].String) };
				i32 value{ sp[1].Int };
				scrItoa(buf, value);
				scrAppendString(dest, LoadImm8, buf);
			NEXT_INSN;

			CASE(OP_TEXT_LABEL_COPY) FETCH_INSN;
				sp -= 3;
				rage::scrValue* dest{ sp[3].Reference };
				i32 destSize{ sp[2].Int };
				i32 srcSize{ sp[1].Int };
				while (srcSize > destSize) {
					--srcSize;
					--sp;
				}
				for (i32 i{}; i != srcSize; ++i)
					dest[srcSize - 1 - i].Any = (sp--)->Any;
				LPSTR cDest{ (LPSTR)dest };
				if (cDest)
					cDest[(srcSize * sizeof(rage::scrValue)) - 1] = '\0';
			NEXT_INSN;

			CASE(OP_CATCH) FETCH_INSN;
				Serialised->m_catch_pointer_count = (i32)(pc - opcodes);
				Serialised->m_catch_frame_pointer = (i32)(fp - Stack);
				Serialised->m_catch_stack_pointer = (i32)(sp - Stack + 1);
				++sp;
				sp[0].Int = -1;
			NEXT_INSN;

			CASE(OP_THROW) FETCH_INSN;
				i32 imm{ sp[0].Int};
				if (!Serialised->m_catch_pointer_count) {
					FAULT("THROW with no CATCH");
				}
				else {
					SET_PC(Serialised->m_catch_pointer_count);
					fp = Stack + Serialised->m_catch_frame_pointer;
					sp = Stack + Serialised->m_catch_stack_pointer;
					sp[0].Int = imm;
				}
			NEXT_INSN;

			CASE(OP_CALLINDIRECT) FETCH_INSN;
				u32 imm{ sp[0].Uns };
				if (!imm)
					FAULT("Attempted to call through uninitialized (zero) function pointer");
				sp[0].Uns = (i32)(pc - opcodes);
				SET_PC(imm);
			NEXT_INSN;

			CASE(OP_PUSH_CONST_M1) FETCH_INSN; ++sp; sp[0].Int = -1; NEXT_INSN;
			CASE(OP_PUSH_CONST_0) FETCH_INSN; ++sp; sp[0].Any = 0; NEXT_INSN;
			CASE(OP_PUSH_CONST_1) FETCH_INSN; ++sp; sp[0].Int = 1; NEXT_INSN;
			CASE(OP_PUSH_CONST_2) FETCH_INSN; ++sp; sp[0].Int = 2; NEXT_INSN;
			CASE(OP_PUSH_CONST_3) FETCH_INSN; ++sp; sp[0].Int = 3; NEXT_INSN;
			CASE(OP_PUSH_CONST_4) FETCH_INSN; ++sp; sp[0].Int = 4; NEXT_INSN;
			CASE(OP_PUSH_CONST_5) FETCH_INSN; ++sp; sp[0].Int = 5; NEXT_INSN;
			CASE(OP_PUSH_CONST_6) FETCH_INSN; ++sp; sp[0].Int = 6; NEXT_INSN;
			CASE(OP_PUSH_CONST_7) FETCH_INSN; ++sp; sp[0].Int = 7; NEXT_INSN;

			CASE(OP_PUSH_CONST_FM1) FETCH_INSN; ++sp; sp[0].Uns = 0xbf800000; NEXT_INSN;
			CASE(OP_PUSH_CONST_F0) FETCH_INSN; ++sp; sp[0].Uns = 0x00000000; NEXT_INSN;
			CASE(OP_PUSH_CONST_F1) FETCH_INSN; ++sp; sp[0].Uns = 0x3f800000; NEXT_INSN;
			CASE(OP_PUSH_CONST_F2) FETCH_INSN; ++sp; sp[0].Uns = 0x40000000; NEXT_INSN;
			CASE(OP_PUSH_CONST_F3) FETCH_INSN; ++sp; sp[0].Uns = 0x40400000; NEXT_INSN;
			CASE(OP_PUSH_CONST_F4) FETCH_INSN; ++sp; sp[0].Uns = 0x40800000; NEXT_INSN;
			CASE(OP_PUSH_CONST_F5) FETCH_INSN; ++sp; sp[0].Uns = 0x40a00000; NEXT_INSN;
			CASE(OP_PUSH_CONST_F6) FETCH_INSN; ++sp; sp[0].Uns = 0x40c00000; NEXT_INSN;
			CASE(OP_PUSH_CONST_F7) FETCH_INSN; ++sp; sp[0].Uns = 0x40e00000; NEXT_INSN;

			CASE(OP_STRINGHASH) FETCH_INSN; sp[0].Uns = rage::joaat(sp[0].String); NEXT_INSN;

			CASE(OP_IS_BIT_SET) FETCH_INSN;
				--sp;
				sp[0].Int = ((sp[0].Int & (1 << sp[1].Int)) != 0);
			NEXT_INSN;

			S_CASE(default) FETCH_INSN;
				try {
					CALL(scriptVm, Stack, Globals, Program, Serialised);
				}
				catch (std::exception& ex) {
					FAULT("Opcode was not found, SVM call was attempted with fail. Exception: {}", ex.what());
				}
				catch (...) {
					FAULT("Bad opcode");
				}
			NEXT_INSN;
		}
	}
}