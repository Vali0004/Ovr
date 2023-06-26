#pragma once
#include "script/tabs/includes.h"

namespace tabs::self {
	inline void movement() {
		elements::tabItem("Movement", [] {
			cbutton("setAllProofs"_AC);
			ctoggleFloatSlider("run"_TFC, 1.f, 5.f);
			ctoggleFloatSlider("swim"_TFC, 1.f, 5.f);
			ctoggleFloatSlider("stamina"_TFC, 11.f, 20.f);
			ctoggleFloatSlider("staminaRegeneration"_TFC, 1.f, 5.f);
			ctoggleFloatSlider("superRun"_TFC, 1.f, 5.f);
			ctoggleFloatSlider("noClip"_TFC, 1.f, 5.f);
			ccheckbox("gracefulLanding"_TC);
			ccheckbox("superJump"_TC);
			ccheckbox("beastJump"_TC);
		});
	}
}