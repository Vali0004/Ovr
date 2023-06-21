#pragma once
#include "script/tabs/includes.h"

namespace tabs::protections {
	inline void networkEvents() {
		elements::tabItem("Network Events", [] {
			elements::protectionToggle("freezeProtection");
			elements::protectionToggle("ragdollProtection");
			elements::protectionToggle("requestControlProtection");
			elements::protectionToggle("giveWeaponProtection");
			elements::protectionToggle("removeWeaponProtection");
			elements::protectionToggle("removeAllWeaponsProtection");
			elements::protectionToggle("voteKickProtection");
		});
	}
}