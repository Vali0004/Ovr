#pragma once
#include "script/tabs/includes.h"

namespace tabs::protections {
	inline void kicks() {
		elements::menu("Kicks", [] {
			elements::sectionProtectionToggle("allKickProtections");
			elements::protectionToggle("desyncKickProtection");
			elements::protectionToggle("lostConnectionKickProtection");
			elements::protectionToggle("bailKickProtection");
			elements::protectionToggle("nullFunctionKickProtection");
			elements::protectionToggle("arrayOverrunKickProtection");
		});
	}
}