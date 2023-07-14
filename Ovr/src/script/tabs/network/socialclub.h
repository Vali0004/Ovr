#pragma once
#include "script/tabs/includes.h"

namespace tabs::network {
	inline char g_dummy[20]{};
	inline void socialclub() {
		elements::menu("Socialclub", [] {
			elements::inputText("Name or Rockstar ID", g_dummy, 200.f);
			elements::button("convert"_SC->m_name, [] {
				commands::g_engine.primitiveExecute("convert {}", g_dummy);
			}, {});
			elements::button("join"_SC->m_name, [] {
				commands::g_engine.primitiveExecute("join {}", g_dummy);
			}, {});
		});
	}
}