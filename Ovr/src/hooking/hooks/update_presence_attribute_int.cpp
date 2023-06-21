#include "hooking/hooking.h"
#include "util/util.h"

bool hooks::updateAttributeInt(PresenceData* Data, int ProfileIndex, char* Attribute, u64 Value) {
	switch (rage::joaat(Attribute)) {
	case "gstype"_joaat: {
		if (util::network::isOnline()) {
			std::string previousType{ gsTypeToString(g_sessionType) };
			g_sessionType = static_cast<eSessionTypes>(Value);
			std::string newType{ gsTypeToString(g_sessionType) };
			LOG(Session, "Changed session types ({} to {})", previousType, newType);
		}
		else {
			LOG(Session, "Joined a {} session", gsTypeToString(g_sessionType));
		}
	} break;
	}
	RET_CALL_DECL(updateAttributeInt, Data, ProfileIndex, Attribute, Value);
}