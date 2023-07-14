#pragma once
#include "script/tabs/includes.h"
#include "reactions/kick.h"
#include "reactions/crash.h"
#include "reactions/network_event.h"
#include "reactions/script_event.h"

namespace tabs::protections::reactions {
	inline void tab() {
		elements::menu("Reactions", [] {
			reactions::kick();
			reactions::crash();
			reactions::networkEvent();
			reactions::scriptEvent();
		});
	}
}