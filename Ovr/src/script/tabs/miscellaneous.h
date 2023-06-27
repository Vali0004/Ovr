#pragma once
#include "includes.h"
#include "miscellaneous/game.h"
#include "miscellaneous/world.h"

namespace tabs::miscellaneous {
	inline void tab() {
		elements::tabBar("Miscellaneous", [] {
			miscellaneous::game();
			miscellaneous::world();
		});
	}
}