#pragma once
#include "script/tabs/includes.h"

namespace tabs::protections {
	inline void networkEvents() {
		elements::tabItem("Network Events", [] {
			elements::protectionToggle("freezeProtection", true);
			elements::protectionToggle("ragdollProtection");
			elements::protectionToggle("requestControlProtection", true);
			elements::protectionToggle("giveWeaponProtection");
			elements::protectionToggle("removeWeaponProtection", true);
			elements::protectionToggle("removeAllWeaponsProtection");
			elements::protectionToggle("voteKickProtection");
		});
	}
}