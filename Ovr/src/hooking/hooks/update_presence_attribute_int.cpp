#include "hooking/hooking.h"
#include "util/util.h"

bool hooks::updateAttributeInt(PresenceData* Data, int ProfileIndex, char* Attribute, u64 Value) {
	switch (rage::joaat(Attribute)) {
	case "gstype"_joaat: {
		if (util::network::g_manager.online()) {
			std::string previousType{ gsTypeToString(g_sessionType) };
			g_sessionType = static_cast<eSessionTypes>(Value);
			g_statistics.setGameType();
			LOG(Session, "Changed session types ({} to {})", previousType, g_statistics.m_gameType);
		}
		else {
			LOG(Session, "Joined a {} session", gsTypeToString(g_sessionType));
		}
	} break;
	}
	RET_CALL_DECL(updateAttributeInt, Data, ProfileIndex, Attribute, Value);
}