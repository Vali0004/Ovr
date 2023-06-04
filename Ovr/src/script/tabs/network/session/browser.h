#pragma once
#include "script/tabs/includes.h"

namespace tabs::network::session {
	inline void browser() {
		elements::tabItem("Browser", [] {
			elements::section("Filters", [] {

			});
		});
	}
}