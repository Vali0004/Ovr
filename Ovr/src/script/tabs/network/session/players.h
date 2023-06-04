#pragma once
#include "script/tabs/includes.h"
#include "players/selected_player.h"

namespace tabs::network::session::players {
	inline void tab() {
		elements::tabItem("Players", [] {
			elements::listBox("Players", { 350.f, 0.f }, [] {
				if (!util::network::iteratePlayers([](uint16_t id, CNetGamePlayer* player) {
					elements::selectable(player->GetName(), g_selectedPlayer == id, [&] {
						g_selectedPlayer = id;
					});
					return false;
				})) {
					elements::text("You're in story mode! Please join a session.");
				}
			});
			if (util::network::isOnline()) {
				elements::sameLine();
				elements::listBox("SelectedPlayer", { 350.f, 0.f }, [] {
					selectedPlayer::tab();
				});
			}
		});
	}
}