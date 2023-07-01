#include "hooking/hooking.h"

void hooks::sendNetworkEvent(rage::netEventMgr* pEventMgr, rage::netGameEvent* pEvent) {
	if (pEvent) {
		switch (static_cast<ePackedEvents>(pEvent->m_id)) {
		case ePackedEvents::CUpdateFxnEvent: {
			return;
		} break;
		case ePackedEvents::CReportCashSpawnEvent: {
			return;
		} break;
		case ePackedEvents::CNetworkCheckCodeCrcsEvent: {
			return;
		} break;
		}
	}
	RET_CALL(sendNetworkEvent, pEventMgr, pEvent);
}