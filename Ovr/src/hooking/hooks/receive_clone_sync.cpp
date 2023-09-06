#include "hooking/hooking.h"
#include "commands/manager/manager.h"
#include  "rage/tables.h"

inline rage::CBaseModelInfo* getModel(u32 hash) {
	const auto table{ *pointers::g_hashTable };
	for (HashNode* node{ table.m_lookup_table[hash % table.m_lookup_key] }; node; node = node->m_next) {
		if (node->m_hash == hash) {
			if (rage::CBaseModelInfo* model{ table.m_data[node->m_idx] }) {
				return model;
			}
		}
	}
	return nullptr;
}
eAckCode hooks::receiveCloneSync(CNetworkObjectMgr* pObjMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, eNetObjectType ObjectType, u16 ObjectId, rage::datBitBuffer* Buffer, u16 Unknown, u32 Timestamp) {
	if (ObjectType < eNetObjectType::Automobile || ObjectType > eNetObjectType::Train) {
		switch ("invalidObjectTypeCrashProtection"_PC->state()) {
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
			switch ("mismatchObjectTypeCrashProtection"_PC->state()) {
			case eProtectionState::Notify: {
				LOG(Session, "T{} crash from {}", 4, Sender->GetName());
			} break;
			case eProtectionState::Block: {
				return eAckCode::Fail;
			} break;
			case eProtectionState::BlockAndNotify: {
				LOG(Session, "T{} crash from {}", 4, Sender->GetName());
				return eAckCode::Fail;
			} break;
			}
		}
		if (CObject* object{ netObject->GetGameObject() }) {
			if (rage::CBaseModelInfo* modelInfo{ object->m_model_info }) {
				if (getModel(modelInfo->m_hash)->get_type() != (i16)ObjectType) {
					switch ("mismatchObjectTypeCrashProtection"_PC->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "T{} crash from {}", 7, Sender->GetName());
					} break;
					case eProtectionState::Block: {
						return eAckCode::Fail;
					} break;
					case eProtectionState::BlockAndNotify: {
						LOG(Session, "T{} crash from {}", 7, Sender->GetName());
						return eAckCode::Fail;
					} break;
					}
				}
				for (u8 i{}; i != COUNT(tables::g_modelBlacklist); ++i) {
					if (modelInfo->m_hash == tables::g_modelBlacklist[i]) {
						return eAckCode::Fail;
					}
				}
				for (u8 i{}; i != COUNT(tables::g_invalidLodModels); ++i) {
					if (modelInfo->m_hash == tables::g_invalidLodModels[i]) {
						switch ("invalidModelLODCrashProtection"_PC->state()) {
						case eProtectionState::Notify: {
							LOG(Session, "O{} crash from {}", i, Sender->GetName());
						} break;
						case eProtectionState::Block: {
							return eAckCode::Fail;
						} break;
						case eProtectionState::BlockAndNotify: {
							LOG(Session, "O{} crash from {}", i, Sender->GetName());
							return eAckCode::Fail;
						} break;
						}
					}
				}
			}
		}
	}
	RET_CALL(receiveCloneSync, pObjMgr, Sender, Receiver, ObjectType, ObjectId, Buffer, Unknown, Timestamp);
}