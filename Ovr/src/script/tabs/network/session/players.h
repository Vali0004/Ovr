#pragma once
#include "script/tabs/includes.h"
#include "players/selected_player.h"

namespace tabs::network::session::players {
	inline void tab() {
		elements::menu("Players", [] {
			if (util::network::g_manager.online()) {
				for (auto& entry : util::network::g_manager) {
					if (auto& player{ entry.second }; player && player.m_name.size()) {
						elements::menu(player.m_name, [player] {
							g_selectedPlayer = player.m_index;
							selectedPlayer::tab(player);
						});
					}
				}
			}
			else {
				elements::text("You're in story mode! Please join a session.");
			}
		});
	}
}