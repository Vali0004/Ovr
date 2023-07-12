#include "pch/pch.h"
#include "core/core.h"

BOOL APIENTRY DllMain(HMODULE hmod, DWORD callReason, LPVOID reserved) {
	switch (callReason) {
	case DLL_PROCESS_ATTACH:
		global::g_running = true;
		global::g_thread = CreateThread(nullptr, NULL, thread::entry, hmod, NULL, nullptr);
		break;
	case DLL_THREAD_DETACH: //We should be able to forcefully yeet the dll if anything happens.
		global::g_running = false;
		if (global::g_thread)
			CloseHandle(global::g_thread);
		break;
	case DLL_PROCESS_DETACH:
		global::g_running = false;
		if (global::g_thread)
			CloseHandle(global::g_thread);
		break;
	}
	return TRUE;
}