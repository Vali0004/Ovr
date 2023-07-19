#pragma once
#include "script/tabs/includes.h"

namespace tabs::miscellaneous {
	inline void game() {
		elements::menu("Game", [] {
			ccheckbox("mobileRadio"_TC);
			ccheckbox("autoMp"_TC);
			ccheckbox("exitInstantly"_TC);
			cbutton("forceQuitToSp"_AC);
		});
	}
}