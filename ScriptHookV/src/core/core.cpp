#include "pch/pch.h"
#include "core/core.h"
#include "core/joaat.h"
#include "exports/exports.h"

namespace core {
	void initialize() {
		printf("init\n");
	}
	void loop() {
		while (global::g_running) {
			Sleep(5000);
		}
	}
	void uninitialize(HMODULE hmod) {
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
}