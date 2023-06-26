#pragma once
#include "script/tabs/includes.h"

namespace tabs::weapon {
	inline void ammo() {
		elements::tabItem("Ammo", [] {
			elements::section("Special Ammo", [] {
				ccheckbox("explosiveAmmo"_TC);
			});
			ccheckbox("infiniteAmmo"_TC);
			ccheckbox("infiniteClip"_TC);
			cbutton("refillAmmo"_AC);
		});
	}
}