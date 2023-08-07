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
			//There is zero reason to continue execution if it is not valid, just start deallocation
			if (m_isGood) {
				m_labelMode = false;
				m_ip = 0;
				m_opcodesTbl = opcodes;
				callback(*this);
				m_codePageCollection.codePageSize = (m_opcodesTbl - opcodes) + 1;
			}
		}
		
		void nop() {
			OSTART(1)
				PUSH8(OP_NOP)
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
		void pushf(fp val) {
			push<fp>(val);
		}
		template <typename t>
		void tPush(t value) {
			if constexpr (std::is_same_v<t, ccp>) {
				push_string(std::forward<t>(value));
			}
			else if constexpr (std::is_same_v<t, fp>) {
				pushf(std::forward<t>(value));
			}
			else {
				push<t>(std::forward<t>(value));
			}
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
		void ineg() {
			OSTART(1)
				PUSH8(OP_INEG)
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
		void label(const std::string& label) {
			if (m_labelMode) {
				addLabel(label);
			}
		}
		void enter(ccp name, u8 params, u16 locals) {
			if (name != nullptr) {
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
			enter(nullptr, params, locals);
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
		void jgt(ccp label) {
			OSTART(3)
				PUSH8(OP_IGT_JZ)
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
		void push_string(const std::string& string) {
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
		void parseSingleLine(std::string& str, size_t lineCount) {
			if (!str.empty()) {
				if (str == "//> Header Information" || str == "//<" || str == "//>" ||
					str == "//" || str == "SetSignature") {
					return;
				}
				size_t pos{ str.find("//") };
				if (pos == 0) {
					return;
				}
				else if (pos != -1 && pos != str.size() - 2) {
					LOG(Fatal, "L{}: Invalid comment (position {}).", lineCount, pos);
					m_isGood = false;
					return;
				}
				if (str.find(':') != std::string::npos) {
					auto pos = str.find("//>");
					str = str.substr(1);
					if (pos != std::string::npos) {
						if (str.size() - 3 == pos) {
							label(str.substr(0, str.size() - 4).c_str());
						}
						else if (pos == 0) {
							return; //Completely ignore that shit
						}
					}
					else {
						label(str.c_str());
					}
					return;
				}
				if (str.find("Function") != std::string::npos) {
					str = str.substr(sizeof("Function"));
					std::vector<std::string> args{ splitString(str, ' ') };
					u8 paramCount{ static_cast<u8>(std::stoi(args[0])) };
					u16 localCount{ static_cast<u16>(std::stoi(args[1])) };
					function(paramCount, localCount);
					return;
				}
				if (str.find("Return") != std::string::npos) {
					str = str.substr(sizeof("Return"));
					std::vector<std::string> args{ splitString(str, ' ') };
					u8 paramCount{ static_cast<u8>(std::stoi(args[0])) };
					u8 returnCount{ static_cast<u8>(std::stoi(args[1])) };
					leave(paramCount, returnCount);
					return;
				}
				if (str.find("CallNative") != std::string::npos) {
					str = str.substr(sizeof("CallNative"));
					std::vector<std::string> args{ splitString(str, ' ') };
					std::string nativeName{ args[0] };
					u64 hash{ nativeFromName(nativeName.c_str()) };
					if (!hash) {
						LOG(Fatal, "L{}: Invalid native '{}'.", lineCount, nativeName);
						m_isGood = false;
						return;
					}
					u8 paramCount{ static_cast<u8>(std::stoi(args[1])) };
					u8 returnCount{ static_cast<u8>(std::stoi(args[2])) };
					native(hash, paramCount, returnCount);
					return;
				}
				if (str.find("PushString") != std::string::npos) {
					str = str.substr(sizeof("PushString"));
					if (str[0] != '\"') {
						LOG(Fatal, "L{}: Missing begining quote.", lineCount);
						m_isGood = false;
						return;
					}
					else if (str[str.size() - 1] != '\"') {
						LOG(Fatal, "L{}: Missing end quote.", lineCount);
						m_isGood = false;
						return;
					}
					LOG_DEBUG("String: {}", str);
					LOG_DEBUG("push_string: {}", str.substr(1, str.size() - 2));
					push_string(str.substr(1, str.size() - 2));
					return;
				}
				if (str.find("PushF") != std::string::npos) {
					str = str.substr(sizeof("PushF"));
					pushf(std::stof(str));
					return;
				}
				if (str.find("PushB2") != std::string::npos) {
					str = str.substr(sizeof("PushB2"));
					std::vector<std::string> args{ getMatches(str, R"_(\b\d{1,3}\b)_") };
					if (args.size() != 2) {
						LOG(Fatal, "L{}: Missing arugment (PushB2 expects 2 arguments, it received {})", lineCount, args.size());
						m_isGood = false;
						return;
					}
					u8 b1{ (u8)stoi(args[0]) };
					u8 b2{ (u8)stoi(args[1]) };
					push_b2(b1, b2);
					return;
				}
				if (str.find("PushB3") != std::string::npos) {
					str = str.substr(sizeof("PushB3"));
					std::vector<std::string> args{ getMatches(str, R"_(\b\d{1,3}\b)_") };
					if (args.size() != 3) {
						LOG(Fatal, "L{}: Missing arugment (PushB3 expects 3 arguments, it received {})", lineCount, args.size());
						m_isGood = false;
						return;
					}
					u8 b1{ (u8)stoi(args[0]) };
					u8 b2{ (u8)stoi(args[1]) };
					u8 b3{ (u8)stoi(args[2]) };
					push_b3(b1, b2, b3);
					return;
				}
				if (str.find("Push") != std::string::npos) {
					str = str.substr(sizeof("Push"));
					std::vector<std::string> args{ splitString(str, ' ') };
					if (args.size() != 1) {
						LOG(Fatal, "L{}: Missing arugment (Push expects 1 argument, it received {})", lineCount, args.size());
						m_isGood = false;
						return;
					}
					if (isNumber(args[0])) {
						u32 value{ (u32)stoi(args[0]) };
						push<u32>(value);
					}
					else {
						if (args[0] == "TRUE") {
							push<u32>(TRUE);
						}
						else if (args[0] == "FALSE") {
							push<u32>(FALSE);
						}
						else {
							LOG(Fatal, "L{}: Invalid push argument ({})", lineCount, args[0]);
							m_isGood = false;
						}
					}
					return;
				}
				if (str.find("Jump") != std::string::npos) {
					str = str.substr(sizeof("Jump "));
					if (str.empty()) {
						LOG(Fatal, "L{}: Missing arugment (Jump expects 1 arugment)", lineCount);
						m_isGood = false;
						return;
					}
					else if (!m_labels.count(str)) {
						LOG(Fatal, "L{}: Invalid Jump label", lineCount);
						m_isGood = false;
						return;
					}
					jmp(str.c_str());
					return;
				}
				if (str.find("JumpFalse") != std::string::npos) {
					str = str.substr(sizeof("JumpFalse "));
					if (str.empty()) {
						LOG(Fatal, "L{}: Missing arugment (JumpFalse expects 1 arugment)", lineCount);
						m_isGood = false;
						return;
					}
					else if (!m_labels.count(str)) {
						LOG(Fatal, "L{}: Invalid JumpFalse label", lineCount);
						m_isGood = false;
						return;
					}
					jz(str.c_str());
					return;
				}
				if (str.find("JumpGT") != std::string::npos) {
					str = str.substr(sizeof("JumpGT "));
					if (str.empty()) {
						LOG(Fatal, "L{}: Missing arugment (JumpGT expects 1 arugment)", lineCount);
						m_isGood = false;
						return;
					}
					else if (!m_labels.count(str)) {
						LOG(Fatal, "L{}: Invalid JumpGT label", lineCount);
						m_isGood = false;
						return;
					}
					jgt(str.c_str());
					return;
				}
				if (str.find("Nop") != std::string::npos) {
					str = str.substr(sizeof("Nop"));
					nop();
					return;
				}
				if (str.find("Add") != std::string::npos) {
					str = str.substr(sizeof("Add"));
					iadd();
					return;
				}
				if (str.find("Sub") != std::string::npos) {
					str = str.substr(sizeof("Sub"));
					isub();
					return;
				}
				if (str.find("Mult") != std::string::npos) {
					str = str.substr(sizeof("Mult"));
					imul();
					return;
				}
				if (str.find("Div") != std::string::npos) {
					str = str.substr(sizeof("Div"));
					idiv();
					return;
				}
				if (str.find("Mod") != std::string::npos) {
					str = str.substr(sizeof("Mod"));
					imod();
					return;
				}
				if (str.find("Not") != std::string::npos) {
					str = str.substr(sizeof("Not"));
					inot();
					return;
				}
				if (str.find("fAdd") != std::string::npos) {
					str = str.substr(sizeof("fAdd"));
					fadd();
					return;
				}
				if (str.find("fSub") != std::string::npos) {
					str = str.substr(sizeof("fSub"));
					fsub();
					return;
				}
				if (str.find("fMult") != std::string::npos) {
					str = str.substr(sizeof("fMult"));
					fmul();
					return;
				}
				if (str.find("fDiv") != std::string::npos) {
					str = str.substr(sizeof("fDiv"));
					fdiv();
					return;
				}
				if (str.find("fMod") != std::string::npos) {
					str = str.substr(sizeof("fMod"));
					fmod();
					return;
				}
				LOG(Fatal, "L{}: Unsupported opcode '{}' (IP: {})", lineCount, str, m_ip);
				m_isGood = false;
			}
		}
		void fromYSA(std::string s) {
			std::istringstream stream{ s };
			std::string line{};
			size_t lineCount{};
			while (std::getline(stream, line)) {
				parseSingleLine(line, lineCount);
				lineCount++;
			}
		}
	public:
		std::vector<u64> m_natives{};
		stringPageCollection m_stringPageCollection{};
		codePageCollection m_codePageCollection{};
		bool m_isGood{ true };
	private:
		u32 m_currentParams{};
		std::map<std::string, u32> m_labels{};
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
		u32 getOrMakeStringIndex(const std::string& string) {
			return m_stringPageCollection.addString(string.data());
		}
		u32 getLabelInstructionPointer(const std::string& label) {
			for (auto& l : m_labels) {
				if (!l.first.compare(label)) {
					return l.second;
				}
			}
			throw "Label doesn't exist!";
		}
		void addLabel(const std::string& label) {
			m_labels.insert({ std::string(label), m_ip });
		}
	};
}