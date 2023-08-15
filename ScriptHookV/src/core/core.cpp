#include "pch/pch.h"
#include "core/core.h"
#include "core/joaat.h"
#include "exports/exports.h"
#define BRAND "Ovr"
namespace core {
	void initialize() {
	}
	void loop() {
		std::filesystem::path path{ std::getenv("appdata") };
		path /= BRAND;
		path /= "Cache";
		path /= "Unload.txt";
		std::ifstream file{};
		while (global::g_running) {
			file.open(path);
			if (std::filesystem::exists(path) && file.is_open()) {
				file.close();
				std::filesystem::remove(path);
				global::g_running = false;
			}
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