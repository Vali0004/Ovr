#pragma once
#include "script/tabs/includes.h"
#include "commands/engine/engine.h"

namespace tabs::settings {
	inline void game() {
		elements::menu("Game", [] {
			cbutton("unload"_AC);
			cbutton("exit"_AC);
		});
	}
}