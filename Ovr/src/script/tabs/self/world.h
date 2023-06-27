#pragma once
#include "script/tabs/includes.h"

namespace tabs::self {
	inline void world() {
		elements::tabItem("World", [] {
			ccheckbox("walkOnWater"_TC);
			ccheckbox("walkThroughWater"_TC);
			ccheckbox("slowMotion"_TC);
		});
	}
}