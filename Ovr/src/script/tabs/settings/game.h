#pragma once
#include "script/tabs/includes.h"
#include "commands/engine/engine.h"

namespace tabs::settings {
	inline void game() {
		elements::tabItem("Game", [] {
			cbutton("unload"_AC);
			elements::sameLine();
			cbutton("exit"_AC);
		});
	}
}