#pragma once
#include "script/tabs/includes.h"

namespace tabs::protections {
	inline void kicks() {
		elements::tabItem("Kicks", [] {
			elements::protectionToggle("desyncKickProtection");
			elements::protectionToggle("lostConnectionKickProtection");
			elements::protectionToggle("arrayOverrunKickProtection");
		});
	}
}