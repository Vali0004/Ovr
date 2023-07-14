#pragma once
#include "script/tabs/includes.h"
#include "session/browser.h"
#include "session/players.h"
#include "session/all_players.h"
#include "session/starter.h"

namespace tabs::network::session {
	inline void tab() {
		elements::menu("Session (" + g_statistics.m_gameType + ")", [] {
			session::starter();
			session::browser();
			session::players::tab();
			session::allPlayers::tab();
		});
	}
}