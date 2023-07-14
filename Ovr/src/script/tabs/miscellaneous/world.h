#pragma once
#include "script/tabs/includes.h"

namespace tabs::miscellaneous {
	inline void world() {
		elements::menu("World", [] {
			cbutton("teleportToWaypoint"_AC);
		});
	}
}