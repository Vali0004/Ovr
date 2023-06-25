#include "hooking/hooking.h"
#include "commands/features.h"

void* hooks::cTaskJumpConstructor(u64 _This, u32 Flags) {
	if ("superJump"_TF->get(0).toggle)
		Flags |= (u32)eTaskFlags::SuperJump;
	if ("beastJump"_TF->get(0).toggle)
		Flags |= (u32)eTaskFlags::BeastJumpWithSuper;
	RET_CALL(cTaskJumpConstructor, _This, Flags)
}