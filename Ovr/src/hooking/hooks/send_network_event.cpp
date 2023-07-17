#include "hooking/hooking.h"

void hooks::sendNetworkEvent(rage::netEventMgr* pEventMgr, rage::netGameEvent* pEvent) {
	if (pEvent) {
		switch (static_cast<ePackedEvents>(pEvent->m_id)) {
		case ePackedEvents::CUpdateFxnEvent: {
			LOG_DEBUG("Tripped Report Myself");
			pEvent->~netGameEvent();
			if (pEvent)
				delete pEvent;
			return;
		} break;
		case ePackedEvents::CReportCashSpawnEvent: {
			LOG_DEBUG("Tripped Report Cash Spawn");
			pEvent->~netGameEvent();
			if (pEvent)
				delete pEvent;
			return;
		} break;
		case ePackedEvents::CNetworkCheckCodeCrcsEvent: {
			LOG_DEBUG("Tripped Code CRCs");
			pEvent->~netGameEvent();
			if (pEvent)
				delete pEvent;
			return;
		} break;
		}
	}
	RET_CALL(sendNetworkEvent, pEventMgr, pEvent);
}