#include "hooking/hooking.h"
#include "commands/manager/manager.h"
#include "util/util.h"
#include "rage/tables.h"

class syncNodeFinder {
public:
	static const std::vector<nodeId>& find(eNetObjectType type, u64 vft) {
		return m_finder.m_syncTreeConvertion[(i32)type][vft];
	}
	static void init() {
		for (i32 i{ (i32)eNetObjectType::Automobile }; i <= (i32)eNetObjectType::Train; i++) {
			if (i == (i32)eNetObjectType::Trailer)
				continue;
			rage::netSyncTree* tree{ pointers::g_getSyncTreeForType(util::network::getObjectMgr(), i) };
			if (tree->m_child_node_count != m_finder.m_syncTrees[i].size()) {
				LOG(Fatal, "Cache nodes failed ({}, {}, {})", i, tree->m_child_node_count, m_finder.m_syncTrees[i].size());
				throw std::runtime_error("Failed to cache nodes");
			}
			for (i32 j{}; j != tree->m_child_node_count; ++j) {
				const u64 vtable{ *(u64*)tree->m_child_nodes[j] };
				const nodeId nodeId{ m_finder.m_syncTrees[i][j] };
				m_finder.m_syncTreeConvertion[i][vtable].push_back(nodeId);
			}
		}
	}
private:
	static inline nodeFinder m_finder{};
};
inline rage::CDynamicEntity* getEntityAttachedTo(rage::CDynamicEntity* Object) {
	if (!Object || !Object->gap50) {
		return nullptr;
	}
	u64* component{ reinterpret_cast<u64*>(reinterpret_cast<u64>(Object->gap50) + 0x48) };
	if (!component)
		return nullptr;
	i32 attachmentCount{ (i32)(component[0x5C]) & 0xF };
	if (attachmentCount < 2)
		return nullptr;
	return reinterpret_cast<rage::CDynamicEntity*>(component[0x00]);
}
inline bool isAttachmentInfinite(rage::CDynamicEntity* Object, uint16_t AttachedTo) {
	if (!Object)
		return false;
	constexpr u64 reasonableLimit{ 150 };
	u64 i{};
	while (Object && i < reasonableLimit) {
		if (Object->m_net_object && Object->m_net_object->m_object_id == AttachedTo)
			return true;
		Object = getEntityAttachedTo(Object);
		i++;
	}
	return false;
}
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
inline bool crashObjectCheck(CNetGamePlayer* Sender, u32 hash) {
	if (rage::CBaseModelInfo* model{ getModel(hash) }) {
		if (!model->of_any_type({ eModelType::Object, eModelType::Time, eModelType::Weapon, eModelType::Destructable, eModelType::WorldObject, eModelType::Sprinkler, eModelType::Unk65, eModelType::Plant, eModelType::LOD, eModelType::Unk132, eModelType::Building })) {
			return true;
		}
		for (u8 i{}; i != COUNT(tables::g_invalidObjects); ++i) {
			switch ("invalidModelCrashProtection"_PC->state()) {
			case eProtectionState::Notify: {
				LOG(Session, "I{} crash from {}", i, Sender->GetName());
			} break;
			case eProtectionState::Block: {
				return true;
			} break;
			case eProtectionState::BlockAndNotify: {
				LOG(Session, "I{} crash from {}", i, Sender->GetName());
				return true;
			} break;
			}
		}
	}
	return false;
}
inline bool crashVehicleCheck(CNetGamePlayer* Sender, u32 hash) {
	if (rage::CBaseModelInfo* model{ getModel(hash) }) {
		if (!model->of_any_type({ eModelType::Vehicle, eModelType::Unk133 })) {
			return true;
		}
		for (u8 i{}; i != COUNT(tables::g_invalidVehicleModels); ++i) {
			if (model->m_hash == tables::g_invalidVehicleModels[i]) {
				switch ("invalidModelVehicleCrashProtection"_PC->state()) {
				case eProtectionState::Notify: {
					LOG(Session, "I{} crash from {}", i + 1, Sender->GetName());
				} break;
				case eProtectionState::Block: {
					return true;
				} break;
				case eProtectionState::BlockAndNotify: {
					LOG(Session, "I{} crash from {}", i + 1, Sender->GetName());
					return true;
				} break;
				}
			}
		}
	}
	return false;
}
inline bool crashPedCheck(CNetGamePlayer* Sender, u32 hash) {
	if (rage::CBaseModelInfo* model{ getModel(hash) }) {
		if (!model->of_any_type({ eModelType::Ped, eModelType::OnlineOnlyPed })) {
			return true;
		}
		for (u8 i{}; i != COUNT(tables::g_invalidLodModels); ++i) {
			if (model->m_hash == tables::g_invalidLodModels[i]) {
				switch ("invalidModelPedCrashProtection"_PC->state()) {
				case eProtectionState::Notify: {
					LOG(Session, "I{} crash from {}", i + 2, Sender->GetName());
				} break;
				case eProtectionState::Block: {
					return true;
				} break;
				case eProtectionState::BlockAndNotify: {
					LOG(Session, "I{} crash from {}", i + 2, Sender->GetName());
					return true;
				} break;
				}
			}
		}
	}
	return false;
}

bool checkNodes(rage::netSyncNodeBase* pNode, CNetGamePlayer* Sender, rage::netObject* Object) {
	if (pNode->IsParentNode()) {
		for (rage::netSyncNodeBase* child{ pNode->m_first_child }; child; child = child->m_next_sibling) {
			if (checkNodes(child, Sender, Object))
				return true;
		}
	}
	else if (pNode->IsDataNode()) {
		CProjectBaseSyncDataNode* baseSyncNode{ reinterpret_cast<CProjectBaseSyncDataNode*>(pNode) };
		if (!(baseSyncNode->m_flags & 1)) {
			return false;
		}
		const u64 vft{ *(u64*)pNode };
		for (const nodeId id : syncNodeFinder::find((eNetObjectType)Object->m_object_type, vft)) {
			switch (id) {
			case "CVehicleCreationDataNode"_joaat: {
				const auto node{ (CVehicleCreationDataNode*)(pNode) };
			} break;
			case "CDoorCreationDataNode"_joaat: {
				const auto node{ (CDoorCreationDataNode*)(pNode) };
				if (crashObjectCheck(Sender, node->m_model)) {
					return true;
				}
			} break;
			case "CPickupCreationDataNode"_joaat: {
				const auto node{ (CPickupCreationDataNode*)(pNode) };
				if (node->m_custom_model && crashObjectCheck(Sender, node->m_custom_model)) {
					return true;
				}
			} break;
			case "CPhysicalAttachDataNode"_joaat: {
				const auto node{ (CPhysicalAttachDataNode*)(pNode) };
			} break;
			case "CPedCreationDataNode"_joaat: {
				const auto node{ (CPedCreationDataNode*)(pNode) };
				if (crashPedCheck(Sender, node->m_model)) {
					return true;
				}
				else if (node->m_has_prop && crashObjectCheck(Sender, node->m_prop_model)) {
					return true;
				}
			} break;
			case "CPedAttachDataNode"_joaat: {
				const auto node{ (CPedAttachDataNode*)(pNode) };
				if (node->m_attached && node->m_attached_to == Object->m_object_id) {
					switch ("pedAttachCrashProtection"_PC->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "P{} crash from {}", 0, Sender->GetName());
					} break;
					case eProtectionState::Block: {
						return true;
					} break;
					case eProtectionState::BlockAndNotify: {
						LOG(Session, "P{} crash from {}", 0, Sender->GetName());
						return true;
					} break;
					}
				}
			} break;
			case "CObjectCreationDataNode"_joaat: {
				const auto node{ (CObjectCreationDataNode*)(pNode) };
				if (crashObjectCheck(Sender, node->m_model)) {
					return true;
				}
			} break;
			case "CPlayerAppearanceDataNode"_joaat: {
				const auto node{ (CPlayerAppearanceDataNode*)(pNode) };
				if (crashPedCheck(Sender, node->m_model_hash)) {
					return true;
				}
			} break;
			case "CPlayerCreationDataNode"_joaat: {
				const auto node{ (CPlayerCreationDataNode*)(pNode) };
				if (crashPedCheck(Sender, node->m_model)) {
					return true;
				}
			} break;
			case "CSectorDataNode"_joaat: {
				const auto node{ (CSectorDataNode*)(pNode) };
				auto Pos{ node->m_pos };
				if (Pos.x == 712 || Pos.y == 712 || Pos.z == 712) {
					switch ("invalidSectorPositionCrashProtection"_PC->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "P{} crash from {}", 0, Sender->GetName());
					} break;
					case eProtectionState::Block: {
						return true;
					} break;
					case eProtectionState::BlockAndNotify: {
						LOG(Session, "P{} crash from {}", 0, Sender->GetName());
						return true;
					} break;
					}
				}
			} break;
			case "CTrainGameStateDataNode"_joaat: {
				const auto node{ (CTrainGameStateDataNode*)(pNode) };
				if (node->m_track_id < 0 || node->m_track_id >= 27) {
					switch ("invalidTrainTrackIndexCrashProtection"_PC->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "T{} crash from {}", 0, Sender->GetName());
					} break;
					case eProtectionState::Block: {
						return true;
					} break;
					case eProtectionState::BlockAndNotify: {
						LOG(Session, "T{} crash from {}", 0, Sender->GetName());
						return true;
					} break;
					}
				}
			} break;
			case "CVehicleProximityMigrationDataNode"_joaat: {
				const auto node{ (CVehicleProximityMigrationDataNode*)(pNode) };
				if (Object) {
					if (CPed* ped{ util::classes::getPed() }) {
						if (rage::netObject* netObject{ ped->m_net_object }) {
							i16 objectId{ netObject->m_object_id };
							if (!ped->m_vehicle ||
								!ped->m_vehicle->m_net_object ||
								ped->m_vehicle->m_net_object->m_object_id != Object->m_object_id ||
								!util::isInVehicle(ped, ped->m_vehicle)
							) {
								for (u8 i{}; i != 16; ++i) {
									if (node->m_has_occupants[i] && node->m_occupants[i] == objectId) {
										switch ("remoteTeleportProtecton"_PC->state()) {
										case eProtectionState::Notify: {
											LOG(Session, "Remote teleport from {}", Sender->GetName());
										} break;
										case eProtectionState::Block: {
											return true;
										} break;
										case eProtectionState::BlockAndNotify: {
											LOG(Session, "Remote teleport from {}", Sender->GetName());
											return true;
										} break;
										}
									}
								}
							}
						}
					}
				}
			} break;
			case "CPedGameStateDataNode"_joaat: {
				const auto node{ (CPedGameStateDataNode*)(pNode) };
				if (node->m_on_mount) {
					switch ("pedMountFlagCrashProtection"_PC->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "P{} crash from {}", 1, Sender->GetName());
					} break;
					case eProtectionState::Block: {
						return true;
					} break;
					case eProtectionState::BlockAndNotify: {
						LOG(Session, "P{} crash from {}", 1, Sender->GetName());
						return true;
					} break;
					}
				}
				if (node->m_num_equiped_gadgets <= COUNT(node->m_gadget_hash)) {
					switch ("pedStateGadgetOverflowCrashProtection"_PC->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "P{} crash from {}", 4, Sender->GetName());
					} break;
					case eProtectionState::Block: {
						return true;
					} break;
					case eProtectionState::BlockAndNotify: {
						LOG(Session, "P{} crash from {}", 4, Sender->GetName());
						return true;
					} break;
					}
				}
				for (u32 i{}; i != node->m_num_equiped_gadgets; ++i) {
					if (node->m_gadget_hash[i] != "gadget_parachute"_joaat && node->m_gadget_hash[i] != "gadget_nightvision"_joaat) {
						switch ("invalidGadgetCrashProtection"_PC->state()) {
						case eProtectionState::Notify: {
							LOG(Session, "G{} crash from {}", 0, Sender->GetName());
						} break;
						case eProtectionState::Block: {
							return true;
						} break;
						case eProtectionState::BlockAndNotify: {
							LOG(Session, "G{} crash from {}", 0, Sender->GetName());
							return true;
						} break;
						}
					}
				}
			} break;
			case "CVehicleControlDataNode"_joaat: {
				const auto node{ (CVehicleControlDataNode*)(pNode) };
				if (node->m_is_submarine_car) {
					if (CVehicle* vehicle{ reinterpret_cast<CVehicle*>(Object->GetGameObject()) }){
						if (CVehicleModelInfo* modelInfo{ vehicle->get_model_info() }) {
							u32 hash{ modelInfo->m_hash };
							if (hash != "stormberg"_joaat && hash != "toreador"_joaat) {
								switch ("submarineCarCrashProtection"_PC->state()) {
								case eProtectionState::Notify: {
									LOG(Session, "S{} crash from {}", 3, Sender->GetName());
								} break;
								case eProtectionState::Block: {
									return true;
								} break;
								case eProtectionState::BlockAndNotify: {
									LOG(Session, "S{} crash from {}", 3, Sender->GetName());
									return true;
								} break;
								}
							}
						}
					}
				}
			} break;
			}
		}
	}
	return false;
}
bool hooks::canApplyData(rage::netSyncTree* pSyncTree, rage::netObject* pObject) {
	static bool init{ ([] { syncNodeFinder::init(); }(), true) };
	Player senderId{ pObject->m_owner_id };
	util::network::player sender{ util::network::g_manager[senderId] };
	if (pSyncTree->m_child_node_count) {
		if (checkNodes(pSyncTree->m_next_sync_node, sender.m_netGamePlayer, pObject)) {
			return false;
		}
	}
	RET_CALL(canApplyData, pSyncTree, pObject);
}