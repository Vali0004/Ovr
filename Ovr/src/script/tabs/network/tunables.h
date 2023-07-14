#pragma once
#include "script/tabs/includes.h"

namespace tabs::network {
	inline void tunables() {
		elements::menu("Tunables", [] {
			ccheckbox("offRadar"_TC);
		});
	}
}