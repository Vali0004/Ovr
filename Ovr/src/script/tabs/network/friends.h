#pragma once
#include "script/tabs/includes.h"

namespace tabs::network {
	inline void friends() {
		elements::menu("Friends", [] {
			util::network::friends::iterateFriends([](uint32_t id, bool joinable, FriendInfo* info) {
				std::string name{ info->m_name };
				switch (info->m_friend_state) {
				case 1: {
					name += " [Online]";
				} break;
				case 2: {
					name += " [Multiplayer]";
				} break;
				case 3: {
					name += joinable ? " [Multiplayer]" : " [Story]";
				} break;
				default: {
					name += " [Offline]";
				} break;
				}
				elements::menu(name, [&] {
					g_selectedFriend = id;
					FriendInfo& info{ util::network::friends::getFriends()[g_selectedFriend] };
					elements::text("Name: {}", info.m_name);
					elements::text("RID: {}", info.m_rockstar_id);
					cbutton("copyFriendRid"_AC);
					cbutton("copyFriendName"_AC);
					cbutton("joinFriend"_AC);
				});
			});
		});
	}
}