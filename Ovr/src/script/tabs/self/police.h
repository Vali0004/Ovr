#pragma once
#include "script/tabs/includes.h"

namespace tabs::self {
	inline void police() {
		elements::menu("Police", [] {
			cintSlider("wantedLevel"_IC, 0, 5);
			cintSlider("fakeWantedLevel"_IC, 0, 5);
			ccheckbox("lockWantedLevel"_TC);
			ccheckbox("neverWanted"_TC);
		});
	}
}