#include "handler.h"

namespace exceptions {
	uint8_t getInstructionLength(uint8_t* code) {
		auto start = code;
		if (x64::disassembleInstructionCode(code).isValid()) {
			return code - start;
		}
		return NULL;
	}
	bool attemptStackRecovery(PEXCEPTION_POINTERS exceptionInfo) {
		g_recoveryCount++;
		if (g_recoveryCount >= 5)
			g_running = false;
		auto& ctx = exceptionInfo->ContextRecord;
		if (const auto len = getInstructionLength(reinterpret_cast<uint8_t*>(ctx->Rip)); len) {
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
		stackWalker sw(StackWalker::AfterExcept);
		exceptionContext ctx{ exceptionInfo };
		if (!ctx.m_fileoffset.empty()) {
			switch (ctx.m_code) {
			case CONTROL_C_EXIT: { return EXCEPTION_CONTINUE_EXECUTION; } break;
			case EXCEPTION_BREAKPOINT: { return EXCEPTION_CONTINUE_EXECUTION; } break;
			case EXCEPTION_ACCESS_VIOLATION: {
				auto exceptionTypeStr = getExceptionType(ctx.m_type);
				auto exceptionName = g_exceptionErrorReasons[ctx.m_code]() ? g_exceptionErrorReasons[ctx.m_code].what() : std::format("0x{:X}", ctx.m_code);
				LOG(FOREGROUND_RED, "Exception", "The game suffered an fatal exception, you may need to restart the game. ({} at {}, reason of {} was {}{})", exceptionName, ctx.m_fileoffset, exceptionName, exceptionTypeStr, ctx.m_type != 8 && exceptionName != "unknown" ? "" : std::format("0x{:X}", ctx.m_deathAddress));
			} break;
			default: {
				auto exceptionName = g_exceptionErrorReasons[ctx.m_code]() ? g_exceptionErrorReasons[ctx.m_code].what() : std::format("0x{:X}", ctx.m_code);
				LOG(FOREGROUND_RED, "Exception", "The game suffered a exception of unknown severity, you may need to restart the game. ({} at {}, reason of exception is unknown)", exceptionName, ctx.m_fileoffset);
			}
			}
		}
		LOG(FOREGROUND_RED, "Exception", "Dumping registers...");
		ctx.printRegisters();
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
		vecExcepHandlerHandle = AddVectoredExceptionHandler(FALSE, PVECTORED_EXCEPTION_HANDLER(exceptionHandler));
		SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER(unhandledExceptionHandler));
	}
	void uninitExceptionHandler() {
		if (vecExcepHandlerHandle)
			RemoveVectoredExceptionHandler(vecExcepHandlerHandle);
	}
}