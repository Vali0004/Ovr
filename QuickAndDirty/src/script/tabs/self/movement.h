#pragma once
#include "script/tabs/includes.h"

namespace tabs::self {
	inline void movement() {
		elements::tabItem("Movement", [] {
			feature_toggleFloatSlider("run"_TFF, 1.f, 5.f);
			feature_toggleFloatSlider("swim"_TFF, 1.f, 5.f);
			feature_toggleFloatSlider("stamina"_TFF, 11.f, 20.f);
			feature_toggleFloatSlider("staminaRegeneration"_TFF, 1.f, 5.f);
			feature_toggleFloatSlider("superRun"_TFF, 1.f, 5.f);
			feature_toggleFloatSlider("noClip"_TFF, 1.f, 5.f);
			feature_checkbox("gracefulLanding"_TF);
			feature_checkbox("superJump"_TF);
			feature_checkbox("beastJump"_TF);
		});
	}
}