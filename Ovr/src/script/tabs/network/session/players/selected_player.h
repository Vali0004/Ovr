#pragma once
#include "script/tabs/includes.h"
#include "selected_player/teleports.h"
#include "json/json.h"

namespace tabs::network::session::players::selectedPlayer {
	inline bool sendEvent(u64 rid, std::string p) {
		rage::rlGamerHandle handle{ rid };
		auto payloadPtr{ std::make_unique<u8[]>(1536) };
		SecureZeroMemory(payloadPtr.get(), 1536);
		memcpy(payloadPtr.get(), p.c_str(), strlen(p.c_str()));
		bool res{};
		res = pointers::g_sendPresenceEvent(0, &handle, 1, (u8*)p.data(), 1);
		res = pointers::g_sendPresenceEvent(0, &handle, 1, (u8*)p.data(), 0);
		res = pointers::g_sendPresenceEvent(0xFFFFFFFF, &handle, 1, (u8*)p.data(), 1);
		res = pointers::g_sendPresenceEvent(0xFFFFFFFF, &handle, 1, (u8*)p.data(), 0);
		memcpy(payloadPtr.get() + 1024, "self", 4);
		res = pointers::g_sendPresenceEvent(0xFFFFFFFF, &handle, 1, payloadPtr.get(), 1);
		res = pointers::g_sendPresenceEvent(0, &handle, 1, payloadPtr.get(), 1);
		res = pointers::g_sendPresenceEvent(0, &handle, 1, payloadPtr.get(), 0);
		return res;
	}
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
		elements::button("test", [player] {
			nlohmann::json json{};
			json["gm.evt"] = {
				{ "e", "TextMessage" },
				{ "d", { { "ps", "hello!" }, { "gh", util::network::base64Handle(player.m_rid) } } }
			};
			char pBuf[512];
			sprintf_s(pBuf, R"({"gm.evt":{"e":"TextMessage","d":{"ps":"%s","gh":"%s"}}})", "hello!", util::network::base64Handle(player.m_rid).c_str());
			printf("buffer: %s\n", pBuf);
			printf("buffer: %s\n", json.dump().c_str());
			if (!sendEvent(player.m_rid, pBuf)) {
				LOG_DEBUG("Failed to send event.");
			}if (!sendEvent(player.m_rid, json.dump())) {
				LOG_DEBUG("Failed to send event.");
			}
		});
	}
}