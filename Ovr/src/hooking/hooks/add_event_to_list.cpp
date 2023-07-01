#include "hooking/hooking.h"

u64 hooks::addEventToList(u64* pEventPool) {
	u64* fn{ reinterpret_cast<u64*>(_ReturnAddress()) };
	if (fn == pointers::g_reportCashSpawnEvent) {
		return 0;
	}
	if (fn == pointers::g_updateFxnEvent) {
		return 0;
	}
	if (fn == pointers::g_networkCheckCodeCrcsEvent) {
		return 0;
	}
	RET_CALL(addEventToList, pEventPool);
}