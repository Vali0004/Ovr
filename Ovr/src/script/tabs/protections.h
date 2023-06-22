#pragma once
#include "includes.h"
#include "protections/kicks.h"
#include "protections/crashes.h"
#include "protections/network_events.h"
#include "protections/script_events.h"
#include "protections/reactions.h"

namespace tabs::protections {
	inline void tab() {
		elements::tabBar("Protections", [] {
			protections::kicks();
			protections::crashes();
			protections::networkEvents();
			protections::scriptEvents();
			protections::reactions::tab();
		});
	}
}