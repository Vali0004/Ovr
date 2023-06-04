#pragma once
#include "includes.h"

namespace tabs::vehicle {
	inline void tab() {
		elements::tabBar("Vehicle", [] {
			elements::tabItem("Spawner", [] {

			});
		});
	}
}