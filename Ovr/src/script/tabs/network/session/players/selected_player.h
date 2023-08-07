#pragma once
#include "script/tabs/includes.h"
#include "selected_player/teleports.h"
#include "json/json.h"
#include "util/packet.h"

namespace tabs::network::session::players::selectedPlayer {
	inline void tab(util::network::player player) {
		elements::menu("Info", [player] {
			elements::text(player.m_data.m_name);
			elements::text(player.m_data.m_index);
			elements::text(player.m_data.m_host);
			elements::text(player.m_data.m_rid);
			elements::text(player.m_data.m_ip);
			elements::text(player.m_data.m_vehicle);
			elements::text(player.m_data.m_health);
			elements::text(player.m_data.m_position);
			elements::text(player.m_data.m_heading);
		});
	}
}