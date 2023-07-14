#pragma once
#include "includes.h"
#include "vehicle/spawner.h"

namespace tabs::vehicle {
	inline void tab() {
		elements::menu("Vehicle", [] {
			vehicle::spawner();
		});
	}
}