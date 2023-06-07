#pragma once
#include "script/tabs/includes.h"
#include "session/browser.h"
#include "session/players.h"
#include "session/all_players.h"
#include "session/starter.h"

namespace tabs::network::session {
	inline void tab() {
		elements::tabItem("Session (" + gsTypeToString(g_sessionType) + ")", [] {
			elements::tabBar("Session", [] {
				session::starter();
				session::browser();
				session::players::tab();
				session::allPlayers::tab();
			});
		});
	}
}