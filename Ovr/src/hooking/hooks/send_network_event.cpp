#include "hooking/hooking.h"

void hooks::sendNetworkEvent(rage::netEventMgr* pEventMgr, rage::netGameEvent* pEvent) {
	if (pEvent) {
		switch (static_cast<ePackedEvents>(pEvent->m_id)) {
		case ePackedEvents::CUpdateFxnEvent: {
			pEvent->~netGameEvent();
			if (pEvent)
				delete pEvent;
			return;
		} break;
		case ePackedEvents::CReportCashSpawnEvent: {
			pEvent->~netGameEvent();
			if (pEvent)
				delete pEvent;
			return;
		} break;
		case ePackedEvents::CNetworkCheckCodeCrcsEvent: {
			pEvent->~netGameEvent();
			if (pEvent)
				delete pEvent;
			return;
		} break;
		}
	}
	RET_CALL(sendNetworkEvent, pEventMgr, pEvent);
}