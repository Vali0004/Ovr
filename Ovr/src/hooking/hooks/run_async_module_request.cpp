#include "hooking/hooking.h"

void hooks::runAsyncModuleRequest(u64* Module) {
	if (util::checkIns(NULL, *(Module + 0x60), 0xE9)) {
		return;
	}
	RET_CALL(runAsyncModuleRequest, Module)
}