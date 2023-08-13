#include "handler.h"
#include "minhook/hde/hde64.h"
#include "zydis/Zydis.h"
#include "util/util.h"
#include "util/player_mgr.h"
#include "util/statistics.h"

namespace exceptions {
	u8 getInstructionLength(u8* code) {
		u8* start{ code };
		hde64s hde{};
		if (u8 len{ static_cast<u8>(hde64_disasm(reinterpret_cast<void*>(code), &hde)) }) {
			return len;
		}
		else if (x64::disassembleInstructionCode(code).isValid()) {
			return static_cast<u8>(code - start);
		}
		else {
			ZyanU64 opcodeAddress{ reinterpret_cast<u64>(code) };
			ZydisDisassembledInstruction instruction{};
			if (ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, opcodeAddress, reinterpret_cast<void*>(opcodeAddress), 32, &instruction))) {
				return instruction.info.length;
			}
		}
		return NULL;
	}
	bool attemptStackRecovery(PEXCEPTION_POINTERS exceptionInfo) {
		PCONTEXT ctx{ exceptionInfo->ContextRecord };
		g_recoveryCount++;
		if (g_recoveryCount >= 5)
			g_running = false;
		if (const auto len = getInstructionLength(reinterpret_cast<u8*>(ctx->Rip)); len) {
			ctx->Rip += len;
			return true;
		}
		return false;
	}
	std::string getExceptionType(ULONG_PTR type) {
		if (g_exceptionErrorReasons[type]()) {
			return g_exceptionErrorReasons[type].what();
		}
		return "unknown";
	}
	LONG CALLBACK onExceptionCallback(PEXCEPTION_POINTERS exceptionInfo) {
		LOG(Exception, "Game base address: 0x{:X}", reinterpret_cast<u64>(GetModuleHandleA(NULL)));
		stackWalker sw(StackWalker::AfterExcept);
		exceptionContext ctx{ exceptionInfo };
		if (!ctx.m_fileoffset.empty()) {
			std::string name{ g_exceptionTypes[ctx.m_code]() ? g_exceptionTypes[ctx.m_code].str() : std::format("0x{:X}", ctx.m_code) };
			switch (ctx.m_code) {
				case CONTROL_C_EXIT:
				case EXCEPTION_BREAKPOINT:
				case EXCEPTION_SINGLE_STEP: {
					LOG(Exception, "The game has suffered a non-fatal exception, you may disregard this message ({} at {})", name, ctx.m_fileoffset);
					return EXCEPTION_CONTINUE_EXECUTION;
				} break;
				case EXCEPTION_GUARD_PAGE:
				case EXCEPTION_ACCESS_VIOLATION: {
					std::string type{ getExceptionType(ctx.m_type) };
					LOG(Exception, "The game suffered an fatal exception, you may need to restart the game. ({} at {}, reason of {} was {}{})", name, ctx.m_fileoffset, name, type, ctx.m_type != 8 && name != "unknown" ? "" : std::format("0x{:X}", ctx.m_deathAddress));
				} break;
				default: {
					LOG(Exception, "The game suffered a exception of unknown severity, you may need to restart the game. ({} at {}, reason of exception is unknown)", name, ctx.m_fileoffset);
				} break;
			}
		}
		LOG(Exception, "Dumping registers...");
		ctx.printRegisters();
		#ifndef DEBUG
			LOG(Exception, "Showing information...");
			LOG(Exception, "Was In Session: {}", util::network::g_manager.online() ? "Yes" : "No");
			LOG(Exception, "Host: {}", util::network::g_manager.online() ? g_statistics.m_host.m_name : "N/A");
			LOG(Exception, "Coords: {}, {}, {}", util::classes::getPed()->get_position().x, util::classes::getPed()->get_position().y, util::classes::getPed()->get_position().z);
			LOG(Exception, "Last Script: {}", rage::tlsContext::get()->m_script_thread->m_name);
			LOG(Exception, "Last Script Event Sender: {}", util::network::g_manager.online() ? g_statistics.m_lastScriptEventSender.m_name : "N/A");
		#endif
		LOG(Exception, "Showing callstack...");
		sw.ShowCallstack(GetCurrentThread(), &ctx.m_context);
		if (attemptStackRecovery(exceptionInfo)) {
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}
	bool onExceptionCallback_TryExcept(PEXCEPTION_POINTERS exceptionInfo) {
		if (onExceptionCallback(exceptionInfo) != EXCEPTION_CONTINUE_EXECUTION)
			return false;
		return true;
	}
	LONG CALLBACK exceptionHandler(PEXCEPTION_POINTERS exceptionInfo) {
		return onExceptionCallback(exceptionInfo);
	}
	LONG CALLBACK unhandledExceptionHandler(PEXCEPTION_POINTERS exceptionInfo) {
		return onExceptionCallback(exceptionInfo);
	}
	void initExceptionHandler() {
		g_handler = AddVectoredExceptionHandler(TRUE, PVECTORED_EXCEPTION_HANDLER(exceptionHandler));
		SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER(unhandledExceptionHandler));
	}
	void uninitExceptionHandler() {
		if (g_handler) {
			RemoveVectoredExceptionHandler(g_handler);
		}
	}
}