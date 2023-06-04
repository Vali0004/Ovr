#pragma once
#include "script/tabs/includes.h"

namespace tabs::network::session {
	inline void starter() {
		elements::tabItem("Starter", [] {
			elements::listBox("Starter", { 350.f, 0.f }, [] {
				for (int i{}; i != SIZEOF(tables::g_sessions); ++i) {
					auto& session{ tables::g_sessions[i] };
					static auto name{ session.name };
					elements::selectable(name, false, [] {
						features::cmd::g_engine.primitiveExecute("go {}", name);
					});
				}
				elements::selectable("bail"_AF->m_name, false, [] {
					"bail"_AF->run();
				});
			});
		});
	}
}