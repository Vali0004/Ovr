#include "hooking/hooking.h"
#include "util/util.h"

bool hooks::updateAttributeInt(PresenceData* Data, int ProfileIndex, char* Attribute, u64 Value) {
	switch (rage::joaat(Attribute)) {
	case "gstype"_joaat: {
		eSessionTypes previousType{ g_sessionType };
		g_sessionType = static_cast<eSessionTypes>(Value);
		g_statistics.setGameType();
		if (previousType != g_sessionType) {
			if (previousType != eSessionTypes::Offline) {
				LOG(Session, "Changed session types ({} to {})", gsTypeToString(previousType), g_statistics.m_gameType);
			}
			else {
				LOG(Session, "Joined a {} from Story Mode", g_statistics.m_gameType);
			}
		}
		std::string playerCount{ "no" };
		if (g_statistics.m_playerCount)
			playerCount = std::to_string(g_statistics.m_playerCount);
		bool plural{};
		if (!g_statistics.m_playerCount)
			plural = true;
		else if (g_statistics.m_playerCount > 1)
			plural = true;
		if (g_statistics.m_host) {
			std::string host{ g_statistics.m_host.m_name };
			LOG(Session, "Joined a new {} session with {} player{} hosted by {}", g_statistics.m_gameType, playerCount, plural ? "s" : "", host);
		}
		else {
			LOG(Session, "Joined a new {} session with {} player{}", g_statistics.m_gameType, playerCount, plural ? "s" : "");
		}
	} break;
	}
	RET_CALL_DECL(updateAttributeInt, Data, ProfileIndex, Attribute, Value);
}