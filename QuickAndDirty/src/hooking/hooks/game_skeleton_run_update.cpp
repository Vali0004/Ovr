#include "hooking/hooking.h"

u64 hooks::gameSkeletonRunUpdate(rage::gameSkeleton* _This, s32 Type) {
	for (auto list{ _This->m_update_function_list }; list; list->m_next) {
	//	if (list->m_type == Type) {
	//		for (auto entry{ list->m_entry }; entry; entry = entry->m_next_ptr) {
	//			//LOG(FOREGROUND_WHITE, "[rage::gameSkeleton::RunUpdate]", "0x{:X}", entry->m_hash);
	//			//rage::SecEngine::Update
	//			if (entry->m_hash == "rageSecEngine"_joaat) {
	//				return NULL;
	//			}
	//		}
	//	}
	}
	RET_CALL(gameSkeletonRunUpdate, _This, Type);
}