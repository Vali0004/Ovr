#include "pch/pch.h"
#include "core/core.h"
#include "core/joaat.h"
#include "exports/exports.h"

namespace core {
	void initialize() {
		global::g_exportModuleAddress = "QuickAndDirty__dummy_module"_joaat;
		global::g_exportModule = (HMODULE)global::g_exportModuleAddress;
		printf("init\n");
	}
	void loop() {
		while (global::g_running) {
			if (GetAsyncKeyState(VK_DELETE)) {
				printf("unload requested\n");
				global::g_running = false;
			}
			global::g_exportModule = (HMODULE)global::g_exportModuleAddress;
		}
	}
	void uninitialize(HMODULE hmod) {
		printf("deinit\n");
		FreeLibraryAndExitThread(hmod, NULL);
	}
}
namespace thread {
	DWORD entry(LPVOID param) {
		core::initialize();
		core::loop();
		core::uninitialize((HMODULE)param);
		return 0;
	}
	BOOL create(HMODULE hmod, DWORD callReason) {
		switch (callReason) {
		case DLL_PROCESS_ATTACH:
			global::g_running = true;
			global::g_thread = CreateThread(nullptr, NULL, entry, hmod, NULL, nullptr);
			break;
		case DLL_PROCESS_DETACH:
			global::g_running = false;
			CloseHandle(global::g_thread);
			break;
		}
		return TRUE;
	}
}