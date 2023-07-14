#pragma once
#include "script/tabs/includes.h"

namespace tabs::network::session {
	inline void browser() {
		elements::menu("Browser", [] {
			elements::menu("Filters", [] {

			});
		});
	}
}