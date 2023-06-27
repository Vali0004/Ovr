#pragma once
#include "includes.h"
#include "self/ped.h"
#include "self/movement.h"
#include "self/world.h"
#include "self/police.h"

namespace tabs::self {
	inline void tab() {
		elements::tabBar("Self", [] {
			self::ped();
			self::movement();
			self::world();
			self::police();
		});
	}
}