#pragma once
#include "script/tabs/includes.h"

namespace tabs::self {
	inline void movement() {
		elements::tabItem("Movement", [] {
			feature_button("setAllProofs"_AC);
			feature_toggleFloatSlider("run"_TFC, 1.f, 5.f);
			feature_toggleFloatSlider("swim"_TFC, 1.f, 5.f);
			feature_toggleFloatSlider("stamina"_TFC, 11.f, 20.f);
			feature_toggleFloatSlider("staminaRegeneration"_TFC, 1.f, 5.f);
			feature_toggleFloatSlider("superRun"_TFC, 1.f, 5.f);
			feature_toggleFloatSlider("noClip"_TFC, 1.f, 5.f);
			feature_checkbox("gracefulLanding"_TC);
			feature_checkbox("superJump"_TC);
			feature_checkbox("beastJump"_TC);
		});
	}
}