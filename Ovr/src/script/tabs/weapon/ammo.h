#pragma once
#include "script/tabs/includes.h"

namespace tabs::weapon {
	inline void ammo() {
		elements::tabItem("Ammo", [] {
			elements::section("Special Ammo", [] {
				feature_checkbox("explosiveAmmo"_TC);
			});
			feature_checkbox("infiniteAmmo"_TC);
			feature_checkbox("infiniteClip"_TC);
			feature_button("refillAmmo"_AC);
		});
	}
}