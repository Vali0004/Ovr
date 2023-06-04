#pragma once
#include "script/tabs/includes.h"

namespace tabs::weapon {
	inline void ammo() {
		elements::tabItem("Ammo", [] {
			elements::section("Special Ammo", [] {
				feature_checkbox("explosiveAmmo"_TF);
			});
			feature_checkbox("infiniteAmmo"_TF);
			feature_checkbox("infiniteClip"_TF);
			feature_button("refillAmmo"_AF);
		});
	}
}