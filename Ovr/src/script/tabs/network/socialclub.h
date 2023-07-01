#pragma once
#include "script/tabs/includes.h"

namespace tabs::network {
	inline char g_dummy[20]{};
	inline void socialclub() {
		elements::tabItem("Socialclub", [] {
			elements::inputText("##data", g_dummy);
			elements::button("Convert", [] {
				commands::g_engine.primitiveExecute("convert {}", g_dummy);
			}, {}, true);
		});
	}
}