#pragma once
#include "script/tabs/includes.h"

namespace tabs::self {
	inline void police() {
		elements::tabItem("Police", [] {
			cintSlider("wantedLevel"_IC, 0, 5);
			ccheckbox("neverWanted"_TC);
		});
	}
}