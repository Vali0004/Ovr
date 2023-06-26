#pragma once
#include "script/tabs/includes.h"

namespace tabs::network {
	inline void tunables() {
		elements::tabItem("Tunables", [] {
			ccheckbox("offradar"_TC);
		});
	}
}