#pragma once
#include "includes.h"
#include "scripting/script_hook_v.h"

namespace tabs::scripting {
	inline void tab() {
		elements::tabBar("Scripting", [] {
			scripting::scriptHookV();
		});
	}
}