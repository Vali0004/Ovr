#pragma once
#include "script/tabs/includes.h"
#include "players/selected_player.h"

namespace tabs::network::session::players {
	inline void tab() {
		elements::menu("Players", [] {
			for (auto& entry : util::network::g_manager) {
				if (auto& player{ entry.second }; player.valid()) {
					elements::menu(player.m_name, [&] {
						g_selectedPlayer = player.m_index;
						if (util::network::g_manager.online()) {
							util::network::player& player{ util::network::g_manager.m_players[g_selectedPlayer] };
							selectedPlayer::tab(player);
						}
					});
				}
			}
			if (!util::network::g_manager.online()) {
				elements::text("You're in story mode! Please join a session.");
			}
		});
	}
}