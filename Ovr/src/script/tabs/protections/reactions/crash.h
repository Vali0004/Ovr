#pragma once
#include "script/tabs/includes.h"

namespace tabs::protections::reactions {
	inline void crash() {
		elements::tabBar("Crash", [] {

		});
	}
}