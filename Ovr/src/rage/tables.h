#pragma once
#include "pch/pch.h"

namespace tables {
	enum class eSessionType : i32 {
		Public,
		NewPublic,
		ClosedCrew,
		Crew,
		ClosedFriend = 6,
		FindFriend = 9,
		Solo,
		InviteOnly,
		JoinCrew,
		SCTV,
		Leave = -1
	};
	struct SessionType {
		eSessionType type{};
		cc id[22]{};
		cc name[22]{};
	};
	inline constexpr SessionType g_sessions[]{
		{ eSessionType::Public, "public", "Public" },
		{ eSessionType::NewPublic, "new public", "New Public" },
		{ eSessionType::ClosedCrew, "closed crew", "Closed Crew" },
		{ eSessionType::Crew, "crew", "Crew" },
		{ eSessionType::ClosedFriend, "closed friend", "Closed Friend" },
		{ eSessionType::FindFriend, "friends", "Find Friends" },
		{ eSessionType::Solo, "solo", "Solo" },
		{ eSessionType::InviteOnly, "invite", "Invite-Only" },
		{ eSessionType::JoinCrew, "join crew", "Join Crewmates" },
		{ eSessionType::SCTV, "admin", "SCTV" },
		{ eSessionType::Leave, "leave", "Leave" },
	};
}
class nodeId {
public:
	constexpr nodeId() {
		id = 0;
	}
	template<size_t N>
	constexpr nodeId(char const (&pp)[N]) {
		id = rage::constexprJoaatFromArr(pp);
	}
	constexpr operator u32() const {
		return id;
	}
public:
	u32 id{};
};
class nodeFinder {
public:
	static constexpr u64 m_syncTreeCount{ u64(eNetObjectType::Train) + 1 };
	std::array<std::unordered_map<u64, std::vector<nodeId>>, m_syncTreeCount> m_syncTreeConvertion{};
	std::array<std::vector<nodeId>, m_syncTreeCount> m_syncTrees{
		{
			//Automobile
			{
				{"CVehicleCreationDataNode"},
				{"CAutomobileCreationDataNode"},

				{"CGlobalFlagsDataNode"},
				{"CDynamicEntityGameStateDataNode"},
				{"CPhysicalGameStateDataNode"},
				{"CVehicleGameStateDataNode"},

				{"CEntityScriptGameStateDataNode"},
				{"CPhysicalScriptGameStateDataNode"},
				{"CVehicleScriptGameStateDataNode"},
				{"CEntityScriptInfoDataNode"},

				{"CPhysicalAttachDataNode"},
				{"CVehicleAppearanceDataNode"},
				{"CVehicleDamageStatusDataNode"},
				{"CVehicleComponentReservationDataNode"},
				{"CVehicleHealthDataNode"},
				{"CVehicleTaskDataNode"},

				{"CSectorDataNode"},
				{"CSectorPositionDataNode"},
				{"CEntityOrientationDataNode"},
				{"CPhysicalVelocityDataNode"},
				{"CVehicleAngVelocityDataNode"},

				{"CVehicleSteeringDataNode"},
				{"CVehicleControlDataNode"},
				{"CVehicleGadgetDataNode"},

				{"CMigrationDataNode"},
				{"CPhysicalMigrationDataNode"},
				{"CPhysicalScriptMigrationDataNode"},
				{"CVehicleProximityMigrationDataNode"},
			},
			//Bike
			{
				{"CVehicleCreationDataNode"},

				{"CGlobalFlagsDataNode"},
				{"CDynamicEntityGameStateDataNode"},
				{"CPhysicalGameStateDataNode"},
				{"CVehicleGameStateDataNode"},
				{"CBikeGameStateDataNode"},

				{"CEntityScriptGameStateDataNode"},
				{"CPhysicalScriptGameStateDataNode"},
				{"CVehicleScriptGameStateDataNode"},
				{"CEntityScriptInfoDataNode"},

				{"CPhysicalAttachDataNode"},
				{"CVehicleAppearanceDataNode"},
				{"CVehicleDamageStatusDataNode"},
				{"CVehicleComponentReservationDataNode"},
				{"CVehicleHealthDataNode"},
				{"CVehicleTaskDataNode"},

				{"CSectorDataNode"},
				{"CSectorPositionDataNode"},
				{"CEntityOrientationDataNode"},
				{"CPhysicalVelocityDataNode"},
				{"CVehicleAngVelocityDataNode"},

				{"CVehicleSteeringDataNode"},
				{"CVehicleControlDataNode"},
				{"CVehicleGadgetDataNode"},

				{"CMigrationDataNode"},
				{"CPhysicalMigrationDataNode"},
				{"CPhysicalScriptMigrationDataNode"},
				{"CVehicleProximityMigrationDataNode"},
			},
			//Boaat
			{
				{"CVehicleCreationDataNode"},

				{"CGlobalFlagsDataNode"},
				{"CDynamicEntityGameStateDataNode"},
				{"CPhysicalGameStateDataNode"},
				{"CVehicleGameStateDataNode"},
				{"CBoatGameStateDataNode"},

				{"CEntityScriptGameStateDataNode"},
				{"CPhysicalScriptGameStateDataNode"},
				{"CVehicleScriptGameStateDataNode"},
				{"CEntityScriptInfoDataNode"},

				{"CPhysicalAttachDataNode"},
				{"CVehicleAppearanceDataNode"},
				{"CVehicleDamageStatusDataNode"},
				{"CVehicleComponentReservationDataNode"},
				{"CVehicleHealthDataNode"},
				{"CVehicleTaskDataNode"},

				{"CSectorDataNode"},
				{"CSectorPositionDataNode"},
				{"CEntityOrientationDataNode"},
				{"CPhysicalVelocityDataNode"},
				{"CVehicleAngVelocityDataNode"},

				{"CVehicleSteeringDataNode"},
				{"CVehicleControlDataNode"},
				{"CVehicleGadgetDataNode"},

				{"CMigrationDataNode"},
				{"CPhysicalMigrationDataNode"},
				{"CPhysicalScriptMigrationDataNode"},
				{"CVehicleProximityMigrationDataNode"},
			},
			//Door
			{
				{"CDoorCreationDataNode"},

				{"CGlobalFlagsDataNode"},
				{"CDoorScriptInfoDataNode"},
				{"CDoorScriptGameStateDataNode"},

				{"CDoorMovementDataNode"},

				{"CMigrationDataNode"},
				{"CPhysicalScriptMigrationDataNode"},
			},
			//Heli
			{
				{"CVehicleCreationDataNode"},
				{"CAutomobileCreationDataNode"},

				{"CGlobalFlagsDataNode"},
				{"CDynamicEntityGameStateDataNode"},
				{"CPhysicalGameStateDataNode"},
				{"CVehicleGameStateDataNode"},

				{"CEntityScriptGameStateDataNode"},
				{"CPhysicalScriptGameStateDataNode"},
				{"CVehicleScriptGameStateDataNode"},
				{"CEntityScriptInfoDataNode"},

				{"CPhysicalAttachDataNode"},
				{"CVehicleAppearanceDataNode"},
				{"CVehicleDamageStatusDataNode"},
				{"CVehicleComponentReservationDataNode"},
				{"CVehicleHealthDataNode"},
				{"CVehicleTaskDataNode"},
				{"CHeliHealthDataNode"},

				{"CSectorDataNode"},
				{"CSectorPositionDataNode"},
				{"CEntityOrientationDataNode"},
				{"CPhysicalVelocityDataNode"},
				{"CVehicleAngVelocityDataNode"},

				{"CVehicleSteeringDataNode"},
				{"CVehicleControlDataNode"},
				{"CVehicleGadgetDataNode"},
				{"CHeliControlDataNode"},

				{"CMigrationDataNode"},
				{"CPhysicalMigrationDataNode"},
				{"CPhysicalScriptMigrationDataNode"},
				{"CVehicleProximityMigrationDataNode"},
			},
			//Object
			{
				{"CObjectCreationDataNode"},

				{"CGlobalFlagsDataNode"},
				{"CDynamicEntityGameStateDataNode"},
				{"CPhysicalGameStateDataNode"},
				{"CObjectGameStateDataNode"},

				{"CEntityScriptGameStateDataNode"},
				{"CPhysicalScriptGameStateDataNode"},
				{"CObjectScriptGameStateDataNode"},
				{"CEntityScriptInfoDataNode"},

				{"CPhysicalAttachDataNode"},
				{"CPhysicalHealthDataNode"},

				{"CSectorDataNode"},
				{"CObjectSectorPosNode"},
				{"CObjectOrientationNode"},
				{"CPhysicalVelocityDataNode"},
				{"CPhysicalAngVelocityDataNode"},

				{"CMigrationDataNode"},
				{"CPhysicalMigrationDataNode"},
				{"CPhysicalScriptMigrationDataNode"},
			},
			//Ped
			{
				{"CPedCreationDataNode"},
				{"CPedScriptCreationDataNode"},

				{"CGlobalFlagsDataNode"},
				{"CDynamicEntityGameStateDataNode"},
				{"CPhysicalGameStateDataNode"},
				{"CPedGameStateDataNode"},
				{"CPedComponentReservationDataNode"},

				{"CEntityScriptGameStateDataNode"},
				{"CPhysicalScriptGameStateDataNode"},
				{"CPedScriptGameStateDataNode"},
				{"CEntityScriptInfoDataNode"},

				{"CPedAttachDataNode"},
				{"CPedHealthDataNode"},
				{"CPedMovementGroupDataNode"},
				{"CPedAIDataNode"},
				{"CPedAppearanceDataNode"},

				{"CPedOrientationDataNode"},
				{"CPedMovementDataNode"},

				{"CPedTaskTreeDataNode"},
				{"CPedTaskSpecificDataNode"},
				{"CPedTaskSpecificDataNode"},
				{"CPedTaskSpecificDataNode"},
				{"CPedTaskSpecificDataNode"},
				{"CPedTaskSpecificDataNode"},
				{"CPedTaskSpecificDataNode"},
				{"CPedTaskSpecificDataNode"},
				{"CPedTaskSpecificDataNode"},

				{"CSectorDataNode"},
				{"CPedSectorPosMapNode"},
				{"CPedSectorPosNavMeshNode"},

				{"CMigrationDataNode"},
				{"CPhysicalMigrationDataNode"},
				{"CPhysicalScriptMigrationDataNode"},
				{"CPedInventoryDataNode"},
				{"CPedTaskSequenceDataNode"},
			},
			//Pickup
			{
				{"CPickupCreationDataNode"},

				{"CGlobalFlagsDataNode"},
				{"CDynamicEntityGameStateDataNode"},

				{"CPickupScriptGameStateNode"},
				{"CPhysicalGameStateDataNode"},
				{"CEntityScriptGameStateDataNode"},
				{"CPhysicalScriptGameStateDataNode"},
				{"CEntityScriptInfoDataNode"},
				{"CPhysicalHealthDataNode"},

				{"CPhysicalAttachDataNode"},

				{"CSectorDataNode"},
				{"CPickupSectorPosNode"},
				{"CEntityOrientationDataNode"},
				{"CPhysicalVelocityDataNode"},
				{"CPhysicalAngVelocityDataNode"},

				{"CMigrationDataNode"},
				{"CPhysicalMigrationDataNode"},
				{"CPhysicalScriptMigrationDataNode"},
			},
			//Pickup Placement
			{
				{"CPickupPlacementCreationDataNode"},
				{"CMigrationDataNode"},
				{"CGlobalFlagsDataNode"},
				{"CPickupPlacementStateDataNode"},
			},
			//Plane
			{
				{"CVehicleCreationDataNode"},

				{"CGlobalFlagsDataNode"},
				{"CDynamicEntityGameStateDataNode"},
				{"CPhysicalGameStateDataNode"},
				{"CVehicleGameStateDataNode"},

				{"CEntityScriptGameStateDataNode"},
				{"CPhysicalScriptGameStateDataNode"},
				{"CVehicleScriptGameStateDataNode"},
				{"CEntityScriptInfoDataNode"},

				{"CPhysicalAttachDataNode"},
				{"CVehicleAppearanceDataNode"},
				{"CVehicleDamageStatusDataNode"},
				{"CVehicleComponentReservationDataNode"},
				{"CVehicleHealthDataNode"},
				{"CVehicleTaskDataNode"},
				{"CPlaneGameStateDataNode"},

				{"CSectorDataNode"},
				{"CSectorPositionDataNode"},
				{"CEntityOrientationDataNode"},
				{"CPhysicalVelocityDataNode"},
				{"CVehicleAngVelocityDataNode"},

				{"CVehicleSteeringDataNode"},
				{"CVehicleControlDataNode"},
				{"CVehicleGadgetDataNode"},
				{"CPlaneControlDataNode"},

				{"CMigrationDataNode"},
				{"CPhysicalMigrationDataNode"},
				{"CPhysicalScriptMigrationDataNode"},
				{"CVehicleProximityMigrationDataNode"},
			},
			//Submarine
			{
				{"CVehicleCreationDataNode"},

				{"CGlobalFlagsDataNode"},
				{"CDynamicEntityGameStateDataNode"},
				{"CPhysicalGameStateDataNode"},
				{"CVehicleGameStateDataNode"},
				{"CSubmarineGameStateDataNode"},

				{"CEntityScriptGameStateDataNode"},
				{"CPhysicalScriptGameStateDataNode"},
				{"CVehicleScriptGameStateDataNode"},
				{"CEntityScriptInfoDataNode"},

				{"CPhysicalAttachDataNode"},
				{"CVehicleAppearanceDataNode"},
				{"CVehicleDamageStatusDataNode"},
				{"CVehicleComponentReservationDataNode"},
				{"CVehicleHealthDataNode"},
				{"CVehicleTaskDataNode"},

				{"CSectorDataNode"},
				{"CSectorPositionDataNode"},
				{"CEntityOrientationDataNode"},
				{"CPhysicalVelocityDataNode"},
				{"CVehicleAngVelocityDataNode"},

				{"CVehicleSteeringDataNode"},
				{"CVehicleControlDataNode"},
				{"CVehicleGadgetDataNode"},
				{"CSubmarineControlDataNode"},

				{"CMigrationDataNode"},
				{"CPhysicalMigrationDataNode"},
				{"CPhysicalScriptMigrationDataNode"},
				{"CVehicleProximityMigrationDataNode"},
			},
			//Player
			{
				{"CPlayerCreationDataNode"},

				{"CGlobalFlagsDataNode"},
				{"CDynamicEntityGameStateDataNode"},
				{"CPhysicalGameStateDataNode"},
				{"CPedGameStateDataNode"},
				{"CPedComponentReservationDataNode"},

				{"CEntityScriptGameStateDataNode"},
				{"CPlayerGameStateDataNode"},

				{"CPedAttachDataNode"},
				{"CPedHealthDataNode"},
				{"CPedMovementGroupDataNode"},
				{"CPedAIDataNode"},
				{"CPlayerAppearanceDataNode"},
				{"CPlayerPedGroupDataNode"},
				{"CPlayerAmbientModelStreamingNode"},
				{"CPlayerGamerDataNode"},
				{"CPlayerExtendedGameStateNode"},

				{"CPedOrientationDataNode"},
				{"CPedMovementDataNode"},

				{"CPedTaskTreeDataNode"},
				{"CPedTaskSpecificDataNode"},
				{"CPedTaskSpecificDataNode"},
				{"CPedTaskSpecificDataNode"},
				{"CPedTaskSpecificDataNode"},
				{"CPedTaskSpecificDataNode"},
				{"CPedTaskSpecificDataNode"},
				{"CPedTaskSpecificDataNode"},
				{"CPedTaskSpecificDataNode"},

				{"CSectorDataNode"},
				{"CPlayerSectorPosNode"},
				{"CPlayerCameraDataNode"},
				{"CPlayerWantedAndLOSDataNode"},

				{"CMigrationDataNode"},
				{"CPhysicalMigrationDataNode"},
				{"CPhysicalScriptMigrationDataNode"},
			},
			//Trailer
			{

			},
			//Train
			{
				{"CVehicleCreationDataNode"},

				{"CGlobalFlagsDataNode"},
				{"CDynamicEntityGameStateDataNode"},
				{"CPhysicalGameStateDataNode"},
				{"CVehicleGameStateDataNode"},
				{"CTrainGameStateDataNode"},

				{"CEntityScriptGameStateDataNode"},
				{"CPhysicalScriptGameStateDataNode"},
				{"CVehicleScriptGameStateDataNode"},
				{"CEntityScriptInfoDataNode"},

				{"CPhysicalAttachDataNode"},
				{"CVehicleAppearanceDataNode"},
				{"CVehicleDamageStatusDataNode"},
				{"CVehicleComponentReservationDataNode"},
				{"CVehicleHealthDataNode"},
				{"CVehicleTaskDataNode"},

				{"CSectorDataNode"},
				{"CSectorPositionDataNode"},
				{"CEntityOrientationDataNode"},
				{"CPhysicalVelocityDataNode"},
				{"CVehicleAngVelocityDataNode"},

				{"CVehicleSteeringDataNode"},
				{"CVehicleControlDataNode"},
				{"CVehicleGadgetDataNode"},

				{"CMigrationDataNode"},
				{"CPhysicalMigrationDataNode"},
				{"CPhysicalScriptMigrationDataNode"},
				{"CVehicleProximityMigrationDataNode"},
			},
		},
	};
};