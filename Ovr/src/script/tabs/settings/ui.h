#pragma once
#include "script/tabs/includes.h"
#include "commands/engine/engine.h"

namespace tabs::settings {
	inline void ui() {
		elements::tabItem("UI", [] {
			feature_floatSlider("scale"_FF, 0.5f, 4.f);
		});
	}
}