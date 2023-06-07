#pragma once
#include "script/tabs/includes.h"

namespace tabs::protections {
	inline void kicks() {
		elements::tabBar("Kicks", [] {
			elements::combo("Breakup", "Disabled", [] {
				
			});
		});
	}
}