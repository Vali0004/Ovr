#pragma once
#include "script/tabs/includes.h"
#include "util/util.h"
#include "shv/dynamic_loader.h"

namespace tabs::scripting {
	inline void scriptHookV() {
		elements::menu("ScriptHookV", [] {
			fs::path path{ fs::path(std::getenv("appdata")).append(BRAND).append("Scripts") };
			util::iteratorFilesInPath(path, ".asi", [](fs::path path, std::string filename) {
				elements::menu(path.stem().string(), [&] {
					g_selectedAsi = path.filename().string();
					elements::text("ASI: {}", g_selectedAsi);
					elements::text("Active: {}", shv::g_asiLoader.isScriptLoaded(g_selectedAsi) ? "Yes" : "No");
					if (shv::g_asiLoader.isScriptLoaded(g_selectedAsi)) {
						elements::button("Unload", [] {
							util::async([&] {
								shv::g_asiLoader.unload(g_selectedAsi);
							});
						});
					}
					else {
						elements::button("Load", [] {
							util::async([&] {
								shv::g_asiLoader.load(g_selectedAsi);
							});
						});
					}
				});
			});
		});
	}
}