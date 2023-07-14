#pragma once
#include "includes.h"
#include "script/script.h"
#include "settings/game.h"
#include "settings/commands.h"
#include "settings/ui.h"

namespace tabs::settings {
	inline void tab() {
		elements::menu("Settings", [] {
			settings::game();
			settings::command();
			settings::ui();
		});
	}
}