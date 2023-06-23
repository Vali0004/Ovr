#include "hooking/hooking.h"
#include "commands/manager/manager.h"
#include "util/util.h"
#include "rage/tables.h"

void storeNodeData() {
	for (i32 i{ (i32)eNetObjectType::Automobile }; i <= (i32)eNetObjectType::Train; i++) {
		if (i == (i32)eNetObjectType::Trailer)
			continue;
		rage::netSyncTree* syncTree{ pointers::g_getSyncTreeForType(util::network::getObjectMgr(), i) };
		if (syncTree->m_child_node_count > datanodes::g_syncTrees[i].second.size()) {
			LOG(Fatal, "Cache nodes failed ({}, {}, {})", i, syncTree->m_child_node_count, datanodes::g_syncTrees[i].second.size());
			throw std::runtime_error("Failed to store node data!");
		}
		for (i32 j{}; j != syncTree->m_child_node_count; ++j) {
			u64 vtable{ *(u64*)syncTree->m_child_nodes[j] };
			for (auto& Node : datanodes::g_nodes) {
				if (Node.first == datanodes::g_syncTrees[i].second[j]) {
					Node.second = (u64)vtable;
					break;
				}
			}
		}
	}
}
u32 getNodeHash(rage::netSyncNodeBase* pNode) {
	u64 vtable{ *(u64*)pNode };
	u32 nodeHash{};
	for (const auto& node : datanodes::g_nodes) {
		if (node.second == vtable) {
			nodeHash = node.first;
			break;
		}
	}
	return nodeHash;
}
bool isAttachmentInfinite(rage::CDynamicEntity* Object, uint16_t AttachedTo) {
	if (!Object)
		return false;
	constexpr u64 reasonableLimit{ 150 };
	u64 i{};
	while (Object && i < reasonableLimit) {
		if (Object->m_net_object && Object->m_net_object->m_object_id == AttachedTo)
			return true;
		Object = pointers::g_getEntityAttachedTo(Object);
		i++;
	}
}
bool checkNodes(rage::netSyncNodeBase* pNode, CNetGamePlayer* Sender, rage::netObject* Object) {
	if (pNode->IsParentNode()) {
		for (rage::netSyncNodeBase* child{ pNode->m_first_child }; child; child = child->m_next_sibling) {
			if (checkNodes(child, Sender, Object))
				return true;
		}
	}
	else if (pNode->IsDataNode()) {
		switch (getNodeHash(pNode)) {
		case "CVehicleCreationDataNode"_joaat: {
			const auto node{ (CVehicleCreationDataNode*)(pNode) };
		} break;
		case "CDoorCreationDataNode"_joaat: {
			const auto node{ (CDoorCreationDataNode*)(pNode) };
		} break;
		case "CPickupCreationDataNode"_joaat: {
			const auto node{ (CPickupCreationDataNode*)(pNode) };
		} break;
		case "CPhysicalAttachDataNode"_joaat: {
			const auto node{ (CPhysicalAttachDataNode*)(pNode) };
		} break;
		case "CPedCreationDataNode"_joaat: {
			const auto node{ (CPedCreationDataNode*)(pNode) };
		} break;
		case "CPedAttachDataNode"_joaat: {
			const auto node{ (CPedAttachDataNode*)(pNode) };
			if (node->m_attached && node->m_attached_to == Object->m_object_id) {
				switch ("pedAttachCrashProtection"_PF->state()) {
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
		} break;
		case "CObjectCreationDataNode"_joaat: {
			const auto node{ (CObjectCreationDataNode*)(pNode) };
		} break;
		case "CPlayerAppearanceDataNode"_joaat: {
			const auto node{ (CPlayerAppearanceDataNode*)(pNode) };
		} break;
		case "CPlayerCreationDataNode"_joaat: {
			const auto node{ (CPlayerCreationDataNode*)(pNode) };
		} break;
		case "CSectorDataNode"_joaat: {
			const auto node{ (CSectorDataNode*)(pNode) };
			auto Pos{ node->m_pos };
			if (Pos.x == 712 || Pos.y == 712 || Pos.z == 712) {
				switch ("invalidSectorPositionCrashProtection"_PF->state()) {
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
				switch ("invalidTrainTrackIndexCrashProtection"_PF->state()) {
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
						if (CVehicle* vehicle{ ped->m_vehicle }) {
							if (util::isInVehicle(ped, vehicle)) {
								if (Object->m_object_id == objectId) {
									switch ("vehicleKickProtection"_PF->state()) {
									case eProtectionState::Notify: {
										LOG(Session, "Vehicle kick from {}", Sender->GetName());
									} break;
									case eProtectionState::Block: {
										return true;
									} break;
									case eProtectionState::BlockAndNotify: {
										LOG(Session, "Vehicle kick from {}", Sender->GetName());
										return true;
									} break;
									}
								}
							}
						}
						if (!ped->m_vehicle ||
							!ped->m_vehicle->m_net_object ||
							ped->m_vehicle->m_net_object->m_object_id != Object->m_object_id ||
							!util::isInVehicle(ped, ped->m_vehicle)
						) {
							for (u8 i{}; i != 16; ++i) {
								if (node->m_has_occupants[i] && node->m_occupants[i] == objectId) {
									switch ("remoteTeleportProtecton"_PF->state()) {
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
		}
	}
	return false;
}
bool hooks::canApplyData(rage::netSyncTree* pSyncTree, rage::netObject* pObject) {
	static bool init{ ([] { storeNodeData(); }(), true) };
	Player senderId{ pObject->m_owner_id };
	util::network::player sender{ util::network::g_manager[senderId] };
	if (pSyncTree->m_child_node_count) {
		if (checkNodes(pSyncTree->m_next_sync_node, sender.m_netGamePlayer, pObject)) {
			return false;
		}
	}
	RET_CALL(canApplyData, pSyncTree, pObject);
}