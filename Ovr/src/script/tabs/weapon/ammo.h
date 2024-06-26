#pragma once
#include "script/tabs/includes.h"

namespace tabs::weapon {
	inline void ammo() {
		elements::menu("Ammo", [] {
			elements::menu("Special Ammo", [] {
				ccheckbox("armorPiercingAmmo"_TC);
				ccheckbox("explosiveAmmo"_TC);
				ccheckbox("fullMetalJacketAmmo"_TC);
				ccheckbox("hollowPointAmmo"_TC);
				ccheckbox("incendiaryAmmo"_TC);
				ccheckbox("tracerAmmo"_TC);
			});
			ccheckbox("infiniteAmmo"_TC);
			ccheckbox("infiniteClip"_TC);
			ccheckbox("infiniteStickyBombs"_TC);
			ccheckbox("infiniteFlares"_TC);
			ccheckbox("teleportGun"_TC);
			ccheckbox("deleteGun"_TC);
			ccheckbox("deadEye"_TC);
			cbutton("refillAmmo"_AC);
		});
	}
}