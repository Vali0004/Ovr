#include "hooking/hooking.h"

u64 hooks::getAvailableMemoryForStreamer(u64* _This) {
	RET_CALL(getAvailableMemoryForStreamer, _This);
}