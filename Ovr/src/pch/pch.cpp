#include "pch/pch.h"
#include "core/logger.h"

void stackWalker::OnOutput(LPCSTR szText) {
	std::string text(szText);
	LOG(Stackwalker, "{}", text.substr(0, text.size() - 1));
}
void stackWalker::OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType, LPCSTR pdbName, ULONGLONG fileVersion) {
	//StackWalker::OnLoadModule(img, mod, baseAddr, size, result, symType, pdbName, fileVersion);
}
void stackWalker::OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName) {
	StackWalker::OnSymInit(szSearchPath, symOptions, szUserName);
}
void stackWalker::OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr) {
	//StackWalker::OnDbgHelpErr(szFuncName, gle, addr);
}
void stackWalker::OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry) {
	if (!entry.lineFileName[0]) {
		if (entry.name[0]) {
			LOG(Stackwalker, "{} (0x{:X}, {}.dll)", entry.name, uint64_t(entry.offset), entry.moduleName);
		}
		return;
	}
}