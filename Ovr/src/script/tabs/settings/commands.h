#pragma once
#include "script/tabs/includes.h"
#include "commands/engine/engine.h"

namespace tabs::settings {
	inline void command() {
		elements::tabItem("Command", [] {
			elements::checkbox("Use direct match results when possible", commands::g_engine.m_useDirectMatchResults);
			elements::checkbox("Use command auto completion when unique enough", commands::g_engine.m_autoComplete);
			elements::checkbox("Use the first result when there is too many results", commands::g_engine.m_useFirstResultOnTooManyResults);
			static char buffer[256];
			elements::inputText("Command", buffer);
			elements::button("Execute", [] {
				std::string strBuf{ buffer };
				commands::g_engine.execute(strBuf);
			});
		});
	}
}