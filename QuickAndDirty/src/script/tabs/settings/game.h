#pragma once
#include "script/tabs/includes.h"
#include "features/manager/command_engine.h"

namespace tabs::settings {
	inline void game() {
		elements::tabItem("Game", [] {
			feature_checkbox("automp"_TF);
			feature_button("unload"_AF);
			elements::sameLine();
			feature_button("exit"_AF);
		});
	}
}