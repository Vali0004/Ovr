#pragma once
#include "script/tabs/includes.h"
#include "commands/engine/engine.h"

namespace tabs::settings {
	inline void command() {
		elements::tabItem("Command", [] {
			ccheckbox("useDirectMatchResults"_TC);
			ccheckbox("autoCompleteCommands"_TC);
			ccheckbox("useFirstCommandOnMultipleResults"_TC);
			ccheckbox("clearCommandBoxOnEnter"_TC);
		});
	}
}