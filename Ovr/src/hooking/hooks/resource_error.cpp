#include "hooking/hooking.h"

void hooks::resourceError(u32 Hash, bool Kill) {
	for (u8 i{}; i != 2; ++i) {
		if (g_errHashes[i] == Hash) {
			Kill = false;
			return;
		}
	}
	//RET_CALL(resourceError, Hash, Kill);
}