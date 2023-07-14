#pragma once
#include "script/tabs/includes.h"

namespace tabs::self {
	inline void movement() {
		elements::menu("Movement", [] {
			ctoggleFloatSlider("run"_TFC, 1.f, 5.f);
			ctoggleFloatSlider("swim"_TFC, 1.f, 5.f);
			ctoggleFloatSlider("stamina"_TFC, 11.f, 20.f);
			ctoggleFloatSlider("staminaRegeneration"_TFC, 1.f, 5.f);
			ctoggleFloatSlider("superRun"_TFC, 1.f, 5.f);
			ctoggleFloatSlider("noClip"_TFC, 1.f, 5.f);
			ccheckbox("autoTeleportToWaypoint"_TC);
			ccheckbox("coordSmash"_TC);
			ccheckbox("walkOnAir"_TC);
			ccheckbox("gracefulLanding"_TC);
			ccheckbox("beastJump"_TC);
			ccheckbox("superJump"_TC);
			ccheckbox("ultraJump"_TC);
		});
	}
}