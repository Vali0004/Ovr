#include "hooking/hooking.h"

bool hooks::dispatchEvent(u64 _This, rage::netConMgr* pConMgr, rage::netConnection::InFrame* pEvent) {
	if (pEvent->GetId() == 0 || pEvent->GetId() == 4) {

	}
	RET_CALL(dispatchEvent, _This, pConMgr, pEvent);
}