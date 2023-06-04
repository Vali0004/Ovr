#pragma once
#include "script/tabs/includes.h"
#include "util/util.h"
#include "features/shv/dynamic_loader.h"

namespace tabs::scripting {
	inline HMODULE g_shv{};
	inline HMODULE g_asi{};
	inline void scriptHookV() {
		elements::tabItem("ScriptHookV", [] {
			elements::listBox("ScriptHookV", { 350.f, 0.f }, [] {
				fs::path path{ fs::path(std::getenv("appdata")).append("Ovr").append("Scripts") };
				util::iteratorFilesInPath(path, ".asi", [](fs::path path, std::string filename) {
					elements::selectable(path.stem().string(), g_selectedAsi == filename, [&] {
						g_selectedAsi = path.filename().string();
					});
				});
			});
			if (!g_selectedAsi.empty()) {
				elements::sameLine();
				elements::listBox("SelectedAsi", { 350.f, 0.f }, [] {
					elements::text("ASI: {}", g_selectedAsi);
					elements::text("Active: {}", GetModuleHandleA(g_selectedAsi.c_str()) ? "Yes" : "No");
					elements::text("Module loaded: {}", GetModuleHandleA("ScriptHookV.dll") ? "Yes" : "No");
					elements::button("Unload", [] {
						g_asi = GetModuleHandleA("ScriptHookV.dll");
						if (g_asi)
							FreeLibrary(g_asi);
					});
					elements::button("Unload module", [] {
						g_asi = GetModuleHandleA(g_selectedAsi.c_str());
						if (g_shv)
							FreeLibrary(g_shv);
					});
					elements::button("Load", [] {
						LOG(FOREGROUND_WHITE, "Info", "ASI Name: {}", g_selectedAsi);
						g_asi = LoadLibraryA(fs::path(std::getenv("appdata")).append("Ovr\\Scripts").append(g_selectedAsi).string().c_str());
					});
					elements::button("Load module", [] {
						g_shv = LoadLibraryA(fs::path(std::getenv("appdata")).append("Ovr\\Modules\\ScriptHookV.dll").string().c_str());
					});
				});
			}
		});
	}
}