#pragma once
#include "script/tabs/includes.h"
#include "util/util.h"
#include "shv/dynamic_loader.h"

namespace tabs::scripting {
	inline HMODULE g_asi{};
	inline void scriptHookV() {
		elements::tabItem("ScriptHookV", [] {
			elements::listBox("ScriptHookV", { 350.f, 0.f }, [] {
				fs::path path{ fs::path(std::getenv("appdata")).append(BRAND).append("Scripts") };
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
					elements::button("Unload", [] {
						util::async([&] {
							shv::g_asiLoader.unload(g_selectedAsi);
						});
					});
					elements::button("Load", [] {
						util::async([&] {
							shv::g_asiLoader.load(g_selectedAsi);
						});
					});
				});
			}
		});
	}
}