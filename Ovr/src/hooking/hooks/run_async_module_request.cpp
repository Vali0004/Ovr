#include "hooking/hooking.h"

void hooks::runAsyncModuleRequest(u64* Module) {
	if (Module == pointers::g_reportModule) {
		return;
	}
	RET_CALL(runAsyncModuleRequest, Module)
}