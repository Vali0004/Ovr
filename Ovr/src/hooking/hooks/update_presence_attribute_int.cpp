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
		std::string playerCount{ "an" };
		if (g_statistics.m_playerCount)
			playerCount = std::to_string(g_statistics.m_playerCount);
		if (g_statistics.m_host && g_statistics.m_host->IsConnected()) {
			std::string host{ g_statistics.m_host->GetName() };
			LOG(Session, "Joined a new {} session with {} player{} hosted by {}", g_statistics.m_gameType, playerCount, g_statistics.m_playerCount == 1 ? "" : "s", host);
		}
		else {
			LOG(Session, "Joined a new {} session with {} player{}", g_statistics.m_gameType, playerCount, g_statistics.m_playerCount == 1 ? "" : "s");
		}
	} break;
	}
	RET_CALL_DECL(updateAttributeInt, Data, ProfileIndex, Attribute, Value);
}