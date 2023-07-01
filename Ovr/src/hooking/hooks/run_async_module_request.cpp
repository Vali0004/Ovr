#include "hooking/hooking.h"

void hooks::runAsyncModuleRequest(u64* Module) {
	if (Module == pointers::g_reportModule) {
		return;
	}
	if (util::checkIns(NULL, *(Module + 0x60), 0xE9)) {
		return;
	}
	RET_CALL(runAsyncModuleRequest, Module)
}