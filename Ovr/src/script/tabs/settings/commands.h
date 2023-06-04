#pragma once
#include "script/tabs/includes.h"
#include "features/manager/command_engine.h"

namespace tabs::settings {
	inline void command() {
		elements::tabItem("Command", [] {
			elements::checkbox("Use direct match results when possible", features::cmd::g_engine.m_useDirectMatchResults);
			elements::checkbox("Use command auto completion when unique enough", features::cmd::g_engine.m_autoComplete);
			elements::checkbox("Use the first result when there is too many results", features::cmd::g_engine.m_useFirstResultOnTooManyResults);
			static char buffer[256];
			elements::inputText("Command", buffer);
			elements::button("Execute", [] {
				std::string strBuf{ buffer };
				features::cmd::g_engine.execute(strBuf);
			});
		});
	}
}