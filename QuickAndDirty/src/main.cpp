#include "core/core.h"

BOOL APIENTRY DllMain(HMODULE hmodule, DWORD  reasonForCall, LPVOID reserved) {
	switch (reasonForCall) {
	case DLL_PROCESS_ATTACH: {
		core::thread::create(hmodule);
	} break;
	case DLL_PROCESS_DETACH: {
		core::thread::destroy();
	} break;
	}
	return TRUE;
}