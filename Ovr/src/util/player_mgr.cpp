#include "player_mgr.h"

namespace util::network {
	void player::update(CNetGamePlayer* netGamePlayer, rage::snPlayer* player, rage::snPeer* peer) {
		m_netGamePlayer = netGamePlayer;
		m_snPlayer = player;
		m_snPeer = peer;
		if (valid()) {
			m_index = m_netGamePlayer->m_player_id;
			m_gamerInfo = m_netGamePlayer->GetGamerInfo();
			m_name = m_netGamePlayer->GetName();
			m_playerInfo = m_netGamePlayer->m_player_info;
			if (m_playerInfo)
				m_ped = m_playerInfo->m_ped;
			if (m_ped)
				m_vehicle = m_ped->m_vehicle;
			if (m_vehicle)
				m_vehicleModelInfo = static_cast<CVehicleModelInfo*>(m_vehicle->m_model_info);
			if (m_ped)
				m_navigation = m_ped->m_navigation;
			if (m_navigation)
				m_pos = m_navigation->m_position;
			if (m_navigation)
				m_heading = m_navigation->m_heading;
			if (m_gamerInfo) {
				m_address = m_gamerInfo->m_public_address;
				m_ip = m_address.m_ip;
				m_port = m_address.m_port;
				m_handle = m_gamerInfo->m_unk_gamer_handle;
				m_rid = m_handle.m_rockstar_id;
				m_peerId = m_gamerInfo->m_peer_id;
				m_peerAddress = m_gamerInfo->m_peer_address;
				m_platformData = m_gamerInfo->m_platform_data;
			}
			m_host = m_netGamePlayer->IsNetworkHost();
			m_data.set(this);
		}
	}
	void manager::loop() {
		if (!online())
			return;
		if (m_playerCount != m_players.size())
			m_players.clear();
		if (mgr()) {
			m_playerCount = mgr()->m_player_count;
			m_playerLimit = mgr()->m_player_limit;
			for (u8 i{ m_playerCount }; i; --i) {
				m_players[i].update(getPlayer(i), session::get()->m_players[i], session::get()->m_peers[i]);
			}
		}
		else {
			m_players.clear();
		}
	}
}