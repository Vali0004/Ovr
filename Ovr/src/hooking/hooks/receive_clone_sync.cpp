#include "hooking/hooking.h"
#include "commands/manager/manager.h"

eAckCode hooks::receiveCloneSync(CNetworkObjectMgr* pObjMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, eNetObjectType ObjectType, u16 ObjectId, rage::datBitBuffer* Buffer, u16 Unknown, u32 Timestamp) {
	if (ObjectType < eNetObjectType::Automobile || ObjectType > eNetObjectType::Train) {
		switch ("invalidObjectTypeCrashProtection"_PF->state()) {
		case eProtectionState::Notify: {
			LOG(Session, "T{} crash from {}", 3, Sender->GetName());
		} break;
		case eProtectionState::Block: {
			return eAckCode::Fail;
		} break;
		case eProtectionState::BlockAndNotify: {
			LOG(Session, "T{} crash from {}", 3, Sender->GetName());
			return eAckCode::Fail;
		} break;
		}
	}
	if (rage::netObject* netObject{ pointers::g_getNetObject(pObjMgr, ObjectId, true) }) {
		if (netObject->m_object_type != (i16)ObjectType) {
			switch ("mismatchObjectTypeCrashProtection"_PF->state()) {
			case eProtectionState::Notify: {
				LOG(Session, "T{} crash from {}", 4, Sender->GetName());
			} break;
			case eProtectionState::Block: {
				netObject->m_object_type = (i16)ObjectType;
			} break;
			case eProtectionState::BlockAndNotify: {
				LOG(Session, "T{} crash from {}", 4, Sender->GetName());
				netObject->m_object_type = (i16)ObjectType;
			} break;
			}
		}
	}
	RET_CALL(receiveCloneSync, pObjMgr, Sender, Receiver, ObjectType, ObjectId, Buffer, Unknown, Timestamp);
}