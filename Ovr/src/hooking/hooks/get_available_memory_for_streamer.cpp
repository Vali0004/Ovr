#include "hooking/hooking.h"

u64 hooks::getAvailableMemoryForStreamer(u64* _This) {
	if (CStreaming::Get()->m_loaded_list_size >= 30000) {
		return 0;
	}
	RET_CALL(getAvailableMemoryForStreamer, _This);
}