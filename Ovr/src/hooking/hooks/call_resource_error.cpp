#include "hooking/hooking.h"

bool hooks::callResourceError(u32 Hash) {
	for (u8 i{}; i != 2; ++i) {
		if (g_errHashes[i] == Hash) {
			return false;
		}
	}
	RET_CALL(callResourceError, Hash);
}