#pragma once
#include "script/tabs/includes.h"

namespace tabs::protections {
	inline void kicks() {
		elements::tabItem("Kicks", [] {
			elements::sectionProtectionToggle("allKickProtections");
			elements::protectionToggle("desyncKickProtection", true);
			elements::protectionToggle("lostConnectionKickProtection");
			elements::protectionToggle("arrayOverrunKickProtection");
		});
	}
}