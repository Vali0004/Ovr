#include "hooking/hooking.h"

u64 hooks::gameSkeletonUpdateBaseRunGroup(rage::gameSkeletonUpdateBase* _This) {
	for (auto entry{ _This->m_child_ptr }; entry; entry = entry->m_next_ptr) {
		//return entry->Run();
		//rage::SecEngine::Update
	}
	RET_CALL(gameSkeletonUpdateBaseRunGroup, _This);
}