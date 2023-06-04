#pragma once
#include "includes.h"
#include "weapon/loadout.h"
#include "weapon/ammo.h"

namespace tabs::weapon {
	inline void tab() {
		elements::tabBar("Weapon", [] {
			weapon::loadout();
			weapon::ammo();
		});
	}
}