#include "hooking/hooking.h"
#include "commands/manager/manager.h"
#include  "rage/tables.h"

bool hooks::receiveCloneCreate(CNetworkObjectMgr* pObjMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, eNetObjectType ObjectType, i32 ObjectId, i32 ObjectFlag, rage::datBitBuffer* Buffer, i32 Timestamp) {
	if (ObjectType < eNetObjectType::Automobile || ObjectType > eNetObjectType::Train) {
		switch ("invalidObjectTypeCrashProtection"_PC->state()) {
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
			switch ("mismatchObjectTypeCrashProtection"_PC->state()) {
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
		if (CObject* object{ netObject->GetGameObject() }) {
			if (rage::CBaseModelInfo* modelInfo{ object->m_model_info }) {
				for (u8 i{}; i != COUNT(tables::g_modelBlacklist); ++i) {
					if (modelInfo->m_hash == tables::g_modelBlacklist[i]) {
						return true;
					}
				}
				for (u8 i{}; i != COUNT(tables::g_invalidLodModels); ++i) {
					if (modelInfo->m_hash == tables::g_invalidLodModels[i]) {
						switch ("invalidModelLODCrashProtection"_PC->state()) {
						case eProtectionState::Notify: {
							LOG(Session, "O{} crash from {}", i + 1, Sender->GetName());
						} break;
						case eProtectionState::Block: {
							return true;
						} break;
						case eProtectionState::BlockAndNotify: {
							LOG(Session, "O{} crash from {}", i + 1, Sender->GetName());
							return true;
						} break;
						}
					}
				}
			}
		}
	}
	RET_CALL(receiveCloneCreate, pObjMgr, Sender, Receiver, ObjectType, ObjectId, ObjectFlag, Buffer, Timestamp);
}