#include "hooking/hooking.h"
#include "commands/manager/manager.h"

bool hooks::receiveCloneCreate(CNetworkObjectMgr* pObjMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, eNetObjectType ObjectType, i32 ObjectId, i32 ObjectFlag, rage::datBitBuffer* Buffer, i32 Timestamp) {
	if (ObjectType < eNetObjectType::Automobile || ObjectType > eNetObjectType::Train) {
		switch ("invalidObjectTypeCrashProtection"_PF->state()) {
		case eProtectionState::Notify: {
			LOG(Session, "T{} crash from {}", 1, Sender->GetName());
		} break;
		case eProtectionState::Block: {
			return true;
		} break;
		case eProtectionState::BlockAndNotify: {
			LOG(Session, "T{} crash from {}", 1, Sender->GetName());
			return true;
		} break;
		}
	}
	if (rage::netObject* netObject{ pointers::g_getNetObject(pObjMgr, ObjectId, true) }) {
		if (netObject->m_object_type != (i16)ObjectType) {
			switch ("mismatchObjectTypeCrashProtection"_PF->state()) {
			case eProtectionState::Notify: {
				LOG(Session, "T{} crash from {}", 2, Sender->GetName());
			} break;
			case eProtectionState::Block: {
				netObject->m_object_type = (i16)ObjectType;
			} break;
			case eProtectionState::BlockAndNotify: {
				LOG(Session, "T{} crash from {}", 2, Sender->GetName());
				netObject->m_object_type = (i16)ObjectType;
			} break;
			}
		}
	}
	RET_CALL(receiveCloneCreate, pObjMgr, Sender, Receiver, ObjectType, ObjectId, ObjectFlag, Buffer, Timestamp);
}