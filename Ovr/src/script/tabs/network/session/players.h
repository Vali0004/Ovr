#pragma once
#include "script/tabs/includes.h"
#include "players/selected_player.h"

namespace tabs::network::session::players {
	inline void tab() {
		elements::tabItem("Players", [] {
			elements::listBox("Players", { 350.f, 0.f }, [] {
				for (auto& entry : util::network::g_manager) {
					if (auto& player{ entry.second }; player.valid()) {
						elements::selectable(player.m_name, g_selectedPlayer == player.m_index, [&] {
							g_selectedPlayer = player.m_index;
						});
					}
				}
				if (!util::network::g_manager.online()) {
					elements::text("You're in story mode! Please join a session.");
				}
			});
			if (util::network::g_manager.online()) {
				elements::sameLine();
				elements::listBox("SelectedPlayer", { 350.f, 0.f }, [] {
					selectedPlayer::tab();
				});
			}
		});
	}
}