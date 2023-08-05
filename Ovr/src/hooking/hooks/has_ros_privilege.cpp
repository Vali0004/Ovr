#include "hooking/hooking.h"

bool hooks::hasRosPrivilege(u64* _This, i32 Privilege) { //Le ban bypass (Fun fact: This with some natives can get you fully to character creation)
	switch (Privilege) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6: { return true; } break;
	case 7: { return false; } break;
	case 8:
	case 9:
	case 10:
	case 11: { return true; } break;
	case 17:
	case 18:
	case 19: { return true; } break;
	case 20:
	case 21:
	case 22:
	case 24:
	case 25: { return false; } break;
	}
	RET_CALL(hasRosPrivilege, _This, Privilege);
}