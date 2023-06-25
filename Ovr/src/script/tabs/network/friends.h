#pragma once
#include "script/tabs/includes.h"

namespace tabs::network {
	inline void friends() {			
		elements::tabItem("Friends", [] {
			elements::listBox("Friends", { 350.f, 0.f }, [] {
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
						name += joinable? " [Multiplayer]" : " [Story]";
					} break;
					default: {
						name += " [Offline]";
					} break;
					}
					elements::selectable(name, g_selectedFriend == id, [&] {
						g_selectedFriend = id;
					});
				});
			});
			elements::sameLine();
			elements::listBox("SelectedFriend", { 350.f, 0.f }, [] {
				static FriendInfo& info{ util::network::friends::getFriends()[g_selectedFriend] };
				elements::text("Name: {}", info.m_name);
				elements::text("RID: {}", info.m_rockstar_id);
				feature_button("copyFriendRid"_AF);
				feature_button("copyFriendName"_AF);
			});
		});
	}
}