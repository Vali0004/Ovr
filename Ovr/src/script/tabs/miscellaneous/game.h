#pragma once
#include "script/tabs/includes.h"

namespace tabs::miscellaneous {
	inline void game() {
		elements::tabItem("Game", [] {
			ccheckbox("mobileRadio"_TC);
			ccheckbox("automp"_TC);
			ccheckbox("exitInstantly"_TC);
			cbutton("forceQuitToSp"_AC);
		});
	}
}