#pragma once
#include "script/tabs/includes.h"

namespace tabs::network {
	inline void tunables() {
		elements::tabItem("Tunables", [] {
			feature_checkbox("offradar"_TF);
		});
	}
}