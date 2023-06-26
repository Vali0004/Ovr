#pragma once
#include "script/tabs/includes.h"
#include "commands/engine/engine.h"

namespace tabs::settings {
	inline void game() {
		elements::tabItem("Game", [] {
			feature_checkbox("automp"_TC);
			feature_button("unload"_AC);
			elements::sameLine();
			feature_button("exit"_AC);
		});
	}
}