#pragma once
#include "script/tabs/includes.h"
#include "selected_player/teleports.h"

namespace tabs::network::session::players::selectedPlayer {
	inline void tab() {
		elements::tabBar("SelectedPlayer", [] {
			selectedPlayer::teleports();
		});
	}
}