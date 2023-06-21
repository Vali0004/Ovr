#include "hooking/hooking.h"
#include "commands/features.h"

void* hooks::cTaskFallConstructor(u64 _This, u32 Flags) {
	if ("gracefulLanding"_TF->v().toggle)
		Flags |= (u32)eTaskFlags::GracefulLanding;
	RET_CALL(cTaskFallConstructor, _This, Flags)
}