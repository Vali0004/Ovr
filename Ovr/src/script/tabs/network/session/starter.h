#pragma once
#include "script/tabs/includes.h"

namespace tabs::network::session {
	inline void starter() {
		elements::menu("Starter", [] {
			elements::listBox("Starter", { 350.f, 0.f }, [] {
				for (int i{}; i != COUNT(tables::g_sessions); ++i) {
					auto& session{ tables::g_sessions[i] };
					auto& name{ session.name };
					elements::selectable(name, false, [name] {
						commands::g_engine.primitiveExecute("go {}", name);
					});
				}
				elements::selectable("bail"_AC->m_name, false, [] {
					"bail"_AC->run();
				});
			});
		});
	}
}