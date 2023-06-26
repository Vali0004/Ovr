#pragma once
#include "script/tabs/includes.h"

namespace tabs::self {
	inline void police() {
		elements::tabItem("Police", [] {
			feature_intSlider("wantedLevel"_IC, 0, 5);
			feature_checkbox("neverWanted"_TC);
		});
	}
}