#include "hooking/hooking.h"

u64 hooks::settingsVramTex(u64* _This, i32 Quality, u64* pSettings) {
	float multiplier = budgeting::getBudgetMultiplier();
	CALL(settingsVramTex, _This, Quality, pSettings);

	//1 GB is the approximate difference between default 'fake settings' amount and our 3 GB assumption
	return pointers::g_vramLocation[Quality + 1] - (1 * g_GB);
}