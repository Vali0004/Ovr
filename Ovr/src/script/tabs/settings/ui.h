#pragma once
#include "script/tabs/includes.h"
#include "commands/engine/engine.h"

namespace tabs::settings {
	inline void ui() {
		elements::tabItem("UI", [] {
			cfloatSlider("scale"_FC, 0.5f, 4.f);
		});
	}
}