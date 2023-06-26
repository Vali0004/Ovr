#pragma once
#include "pch/pch.h"
#include "util/util.h"
#include "rage/classes.h"
#include "rage/commands/list.h"

namespace util::network {
	class player {
	public:
		void update(CNetGamePlayer* netGamePlayer);
	public:
		bool valid() {
			return m_netGamePlayer && m_netGamePlayer->IsConnected() && m_netGamePlayer->m_player_id != UINT8_MAX;
		}
		operator bool() {
			return valid();
		}
	public:
		u8 m_index{};
		std::string m_name{};
		rage::vector3 m_pos{};
		fp m_heading{};
		CPed* m_ped{};
		CVehicle* m_vehicle{};
		CVehicleModelInfo* m_vehicleModelInfo{};
		CNavigation* m_navigation{};
		CPlayerInfo* m_playerInfo{};
		CNetGamePlayer* m_netGamePlayer{};
		rage::snPlayer* m_snPlayer{};
		rage::snPeer* m_snPeer{};
		rage::rlGamerInfo* m_gamerInfo{};
		rage::netSocketAddress m_address{};
		rage::netAddress m_ip{};
		rage::rlGamerHandle m_handle{};
		u64 m_rid{};
		u64 m_peerId{};
		u64 m_peerAddress{};
		u64 m_platformData{};
		u16 m_port{};
		bool m_host{};
	public:
		struct data {
			std::string m_name{};
			std::string m_index{};
			std::string m_host{};
			std::string m_rid{};
			std::string m_ip{};
			std::string m_vehicle{};
			std::string m_health{};
			std::string m_position{};
			std::string m_heading{};
			void set(class player* p) {
				m_name = std::format("Name: {}", p->m_name);
				m_index = std::format("Index: {}", p->m_index);
				m_host = std::format("Host: {}", p->m_host ? "Yes" : "No");
				m_rid = std::format("RID: {}", p->m_rid);
				m_ip = std::format("IP: {}.{}.{}.{}:{}", p->m_ip.m_field1, p->m_ip.m_field2, p->m_ip.m_field3, p->m_ip.m_field4, p->m_port);
				if (p->m_vehicle) {
					m_vehicle = std::format("Vehicle: {}", HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(p->m_vehicleModelInfo->m_manufacturer));
					m_vehicle += " ";
					m_vehicle += HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(p->m_vehicleModelInfo->m_name);
				}
				else {
					m_vehicle = "Vehicle: Not in vehicle";
				}
				if (p->m_ped)
					m_health = std::format("Health: {}/{}", p->m_ped->m_health, p->m_ped->m_maxhealth);
				m_position = std::format("Position: {}, {}, {}", p->m_pos.x, p->m_pos.y, p->m_pos.z);
				m_heading = std::format("Heading: {}", p->m_heading);
			}
		} m_data;
	};
	class manager {
	public:
		void loop();
	public:
		player& get(u16 index) {
			return m_players[index];
		}
		player getByPeerAddress(u64 peerAddress) {
			for (auto& entry : m_players) {
				auto& player{ entry.second };
				if (player.m_peerAddress == peerAddress) {
					return player;
				}
			}
			return {};
		}
		player getByPlatformData(u64 platformData) {
			for (auto& entry : m_players) {
				auto& player{ entry.second };
				if (player.m_platformData == platformData) {
					return player;
				}
			}
			return {};
		}
		player getBySnPlayerPlatformData(u64 platformData) {
			for (auto& entry : m_players) {
				auto& player{ entry.second };
				if (player.m_snPlayer && player.m_snPlayer->m_gamer_info.m_platform_data == platformData) {
					return player;
				}
			}
			return {};
		}
		player getByRid(u64 rid) {
			for (auto& entry : m_players) {
				auto& player{ entry.second };
				if (player.m_rid == rid) {
					return player;
				}
			}
			return {};
		}
		player local() {
			CNetGamePlayer* player{ getLocalPlayer() };
			if (player)
				return get(player->m_player_id);
			return {};
		}
		player host() {
			for (auto& entry : m_players) {
				auto& player{ entry.second };
				if (player.m_host) {
					return player;
				}
			}
			return {};
		}
		player scriptHost() {
			CNetGamePlayer* player{ getScriptHostNetGamePlayer() };
			if (player)
				return get(player->m_player_id);
			return {};
		}
	public:
		player& operator[](u16 index) {
			return get(index);
		}
		std::map<u16, player>::iterator begin() {
			return m_players.begin();
		}
		std::map<u16, player>::iterator end() {
			return m_players.end();
		}
	public:
		bool online() {
			return pointers::g_networkPlayerMgr && mgr() && getLocalPlayer();
		}
	public:
		std::map<u16, player> m_players{};
		u16 m_playerCount{};
		u16 m_playerLimit{};
	private:
		CNetworkPlayerMgr* mgr() {
			return *pointers::g_networkPlayerMgr;
		}
		CNetGamePlayer** players() {
			return mgr()->m_player_list;
		}
		CNetGamePlayer* getPlayer(u16 index) {
			if (m_playerCount > index && mgr()) {
				return players()[index];
			}
			return nullptr;
		}
		CNetGamePlayer* getLocalPlayer() {
			return mgr()->m_local_net_player;
		}
	};
	inline manager g_manager{};
}