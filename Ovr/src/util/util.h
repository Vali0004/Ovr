#pragma once
#include "pch/pch.h"
#include "memory/pointers.h"

namespace util {
	namespace classes {
		inline CPedFactory* getPedFactory() {
			return *pointers::g_pedFactory;
		}
		inline CReplayInterface* getInterface() {
			return *pointers::g_replayInterface;
		}
		inline CVehicleInterface* getVehicleInterface() {
			return getInterface()->m_vehicle_interface;
		}
		inline CPedInterface* getPedInterface() {
			return getInterface()->m_ped_interface;
		}
		inline CPickupInterface* getPickupInterface() {
			return getInterface()->m_pickup_interface;
		}
		inline CObjectInterface* getObjectInterface() {
			return getInterface()->m_object_interface;
		}
		inline CPed* getPed() {
			return getPedFactory()->m_local_ped;
		}
		inline CPlayerInfo* getPlayerInfo() {
			return getPed()->m_player_info;
		}
		inline rage::rlGamerInfo* getRlGamerInfo() {
			return &getPlayerInfo()->m_gamer_info;
		}
	}
	namespace network {
		namespace friends {
			inline FriendRegistry* getRegistry() {
				return pointers::g_friendRegistry;
			}
			inline FriendList* getList() {
				return getRegistry()->m_friend_list;
			}
			inline FriendInfo* getFriends() {
				return getList()->m_friends;
			}
			inline uint32_t getFriendCount() {
				return getRegistry()->m_friend_count;
			}
			inline void iterateFriends(std::function<void(uint32_t, bool, FriendInfo*)> cb) {
				for (uint32_t i{}; i != getFriendCount(); ++i) {
					if (auto fr{ &getFriends()[i] }; fr) {
						cb(i, fr->m_is_joinable, fr);
					}
				}
			}
		}
		namespace socialclub {
			inline ScInfo* getInfo() {
				return pointers::g_scInfo;
			}
			inline std::string getTicket() {
				return getInfo()->m_ticket;
			}
			inline std::string authorizationHeader() {
				return "Authorization: SCAUTH val=\"" + getTicket() + "\"";
			}
			inline ScGame getInfo(const char* id) {
				s32 index{ pointers::g_scGetGameInfoIndex(id, pointers::g_scGameInfo->getGamesAddress(), pointers::g_scGameInfo->m_id) };
				u64 address{ pointers::g_scGameInfo->getGamesAddress() + (index * 0x148i64) };
				ScGame game;
				memcpy(game.pad_0000, (void*)address, sizeof(game.pad_0000));
				game.m_string = (char*)(address + 0x40);
				memcpy(game.pad_0048, (void*)address, sizeof(game.pad_0048));
				return game;
			}
			inline const char* getString(const char* id) {
				ScGame table{ getInfo(id) };
				return table.m_string;
			}
		}
		inline CNetworkPlayerMgr* getNetworkPlayerMgr() {
			return *pointers::g_networkPlayerMgr;
		}
		inline CNetGamePlayer* getLocalNetGamePlayer() {
			return getNetworkPlayerMgr()->m_local_net_player;
		}
		inline CNetGamePlayer** getPlayers() {
			return getNetworkPlayerMgr()->m_player_list;
		}
		inline rage::rlGamerInfo* getNetworkRlGamerInfo() {
			return getLocalNetGamePlayer()->GetGamerInfo();
		}
		inline uint16_t getPlayerCount() {
			return getNetworkPlayerMgr()->m_player_count;
		}
		inline bool isOnline() {
			return pointers::g_networkPlayerMgr && getNetworkPlayerMgr() && getLocalNetGamePlayer() && getPlayerCount();
		}
		inline bool iteratePlayers(std::function<bool(uint16_t, CNetGamePlayer*)> cb) {
			if (isOnline()) {
				for (uint16_t i{}; i != getPlayerCount(); ++i) {
					if (auto& player{ getPlayers()[i] }; player && player->IsConnected()) {
						if (cb(i, player))
							break;
					}
				}
				return true;
			}
			return false;
		}
		inline Network* get() {
			return *pointers::g_network;
		}
	}
	inline void iteratorFilesInPath(fs::path path, std::string ext, std::function<void(fs::path, std::string)> cb) {
		if (fs::exists(path)) {
			fs::directory_iterator iterator{ path.string() };
			for (auto&& entry : iterator) {
				if (entry.is_regular_file()) {
					fs::path path{ entry.path() };
					if (path.has_filename() && path.extension() == ext) {
						cb(path, path.filename().string());
					}
				}
			}
		}
	}
	template <typename t>
	inline int getPoolObjects(int type, int* arr, int arrSize) {
		std::vector<uint64_t> objects{};
		t* inf{};
		switch (type) {
		case 0: {
			inf = (t*)classes::getVehicleInterface();
		} break;
		case 1: {
			inf = (t*)classes::getPedInterface();
		} break;
		case 2: {
			inf = (t*)classes::getObjectInterface();
		} break;
		case 3: {
			inf = (t*)classes::getPickupInterface();
		} break;
		}
		for (int32_t i{}; i != inf->m_count; ++i) {
			if (arrSize == i)
				break;
			objects.push_back(inf->m_list->addr(i));
		}
		for (int32_t i{}; i != objects.size(); ++i) {
			arr[i] = pointers::g_pointerToHandle((rage::CEntity*)objects[i]);
		}
		return objects.size();
	}
	inline std::string getVehicleCounts() {
		auto inf{ classes::getVehicleInterface() };
		return std::format("{}/{}", inf->m_count, inf->m_size);
	}
	inline std::string getPedCounts() {
		auto inf{ classes::getPedInterface() };
		return std::format("{}/{}", inf->m_count, inf->m_size);
	}
	inline std::string getPickupCounts() {
		auto inf{ classes::getPickupInterface() };
		return std::format("{}/{}", inf->m_count, inf->m_size);
	}
	inline std::string getObjectCounts() {
		auto inf{ classes::getObjectInterface() };
		return std::format("{}/{}", inf->m_count, inf->m_size);
	}
	inline std::wstring strToWstr(std::string str) {
		return fs::path(str).wstring();
	}
}