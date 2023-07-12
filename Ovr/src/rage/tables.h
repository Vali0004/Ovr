#pragma once
#include "pch/pch.h"

template<typename t1, typename t2>
struct compile_pair {
public:
	t1 first;
	t2 second;
};
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
	struct scenario {
		std::string_view m_label{};
		std::string_view m_key{};
	};
	inline constexpr scenario g_scenarios[60]{
		{ "Drill", "WORLD_HUMAN_CONST_DRILL" },
		{ "Drink Coffee", "WORLD_HUMAN_AA_COFFEE" },
		{ "Smoke", "WORLD_HUMAN_AA_SMOKE" },
		{ "Binoculars", "WORLD_HUMAN_BINOCULARS" },
		{ "Freeway Bum", "WORLD_HUMAN_BUM_FREEWAY" },
		{ "Slumped Bum", "WORLD_HUMAN_BUM_SLUMPED" },
		{ "Standing Bum", "WORLD_HUMAN_BUM_STANDING" },
		{ "Bum Washing", "WORLD_HUMAN_BUM_WASH" },
		{ "Park Attendant", "WORLD_HUMAN_CAR_PARK_ATTENDANT" },
		{ "Clipboard", "WORLD_HUMAN_CLIPBOARD" },
		{ "Idle Cop", "WORLD_HUMAN_COP_IDLES" },
		{ "Drinking", "WORLD_HUMAN_CHEERING" },
		{ "Drug Dealer", "WORLD_HUMAN_DRUG_DEALER" },
		{ "Drug Dealer (Hard)", "WORLD_HUMAN_DRUG_DEALER_HARD" },
		{ "Mobile Film", "WORLD_HUMAN_MOBILE_FILM_SHOCKING" },
		{ "Leaf Blower", "WORLD_HUMAN_GARDENER_LEAF_BLOWER" },
		{ "Gardener", "WORLD_HUMAN_GARDENER_PLANT" },
		{ "Golf Player", "WORLD_HUMAN_GOLF_PLAYER" },
		{ "Guard Patrol", "WORLD_HUMAN_GUARD_PATROL" },
		{ "Guard Stand", "WORLD_HUMAN_GUARD_STAND" },
		{ "Guard Stand (Military)", "WORLD_HUMAN_GUARD_STAND_ARMY" },
		{ "Hammering", "WORLD_HUMAN_HAMMERING" },
		{ "Hangout Street", "WORLD_HUMAN_HANG_OUT_STREET" },
		{ "Hiker Standing", "WORLD_HUMAN_HIKER_STANDING" },
		{ "Janitor", "WORLD_HUMAN_JOG_STANDING" },
		{ "Jog Standing", "WORLD_HUMAN_HIKER_STANDING" },
		{ "Leaning", "WORLD_HUMAN_LEANING" },
		{ "Maid Cleaning", "WORLD_HUMAN_MAID_CLEAN" },
		{ "Muscle Flex", "WORLD_HUMAN_MUSCLE_FLEX" },
		{ "Free Weights", "WORLD_HUMAN_MUSCLE_FREE_WEIGHTS" },
		{ "Musician", "WORLD_HUMAN_MUSICIAN" },
		{ "Paparazzi", "WORLD_HUMAN_PAPARAZZI" },
		{ "Partying", "WORLD_HUMAN_PARTYING" },
		{ "Picnic", "WORLD_HUMAN_PICNIC" },
		{ "Prostitute (High)", "WORLD_HUMAN_PROSTITUTE_HIGH_CLASS" },
		{ "Prostitute (Low)", "WORLD_HUMAN_PROSTITUTE_LOW_CLASS" },
		{ "Pushups", "WORLD_HUMAN_PUSH_UPS" },
		{ "Security Shine Torch", "WORLD_HUMAN_SECURITY_SHINE_TORCH" },
		{ "Situps", "WORLD_HUMAN_SIT_UPS" },
		{ "Smoking", "WORLD_HUMAN_SMOKING" },
		{ "Smoking Pot", "WORLD_HUMAN_SMOKING_POT" },
		{ "Standing By Fire", "WORLD_HUMAN_STAND_FIRE" },
		{ "Fishining", "WORLD_HUMAN_STAND_FISHING" },
		{ "Impatient", "WORLD_HUMAN_STAND_IMPATIENT" },
		{ "Stand Mobile", "WORLD_HUMAN_STAND_MOBILE" },
		{ "Strip Watch", "WORLD_HUMAN_STRIP_WATCH_STAND" },
		{ "Stupor", "WORLD_HUMAN_STUPOR" },
		{ "Sunbathing (Front)", "WORLD_HUMAN_SUNBATHE" },
		{ "Sunbathing (Back)", "WORLD_HUMAN_SUNBATHE_BACK" },
		{ "Superhero", "WORLD_HUMAN_SUPERHERO" },
		{ "Swimming", "WORLD_HUMAN_SWIMMING" },
		{ "Tennis", "WORLD_HUMAN_TENNIS_PLAYER" },
		{ "Tourist (Map)", "WORLD_HUMAN_TOURIST_MAP" },
		{ "Tourist (Mobile)", "WORLD_HUMAN_TOURIST_MOBILE" },
		{ "Mechine", "WORLD_HUMAN_VEHICLE_MECHANIC" },
		{ "Welding", "WORLD_HUMAN_WELDING" },
		{ "Window Shopping", "WORLD_HUMAN_WINDOW_SHOP_BROWSE" },
		{ "Yoga", "WORLD_HUMAN_YOGA" }
	};
	struct animation {
		std::string_view label{};
		compile_pair<std::string_view, std::string_view> m_animation{};
	};
	inline constexpr animation g_sportAnimations[14]{
		{ "Situps", { "amb@world_human_sit_ups@male@base", "base" } },
		{ "Pushups", { "amb@world_human_push_ups@male@base", "base" } },
		{ "Bench Press", { "amb@prop_human_seat_muscle_bench_press@idle_a", "idle_a" } },
		{ "Pullups", { "SWITCH@FRANKLIN@GYM", "001942_02_GC_FRAS_IG_5_BASE" } },
		{ "Chin Ups 1", { "amb@prop_human_muscle_chin_ups@male@base", "base" } },
		{ "Chin Ups 2", { "amb@prop_human_muscle_chin_ups@male@enter", "enter" } },
		{ "Chin Ups 3", { "amb@prop_human_muscle_chin_ups@male@exit", "exit_flee" } },
		{ "Chin Ups 4", { "amb@prop_human_muscle_chin_ups@male@idle_a", "idle_a" } },
		{ "Free Weights 1", { "amb@world_human_muscle_free_weights@male@barbell@base", "base" } },
		{ "Free Weights 2", { "amb@world_human_muscle_free_weights@male@barbell@idle_a", "idle_a" } },
		{ "Flex 1", { "amb@world_human_muscle_flex@arms_at_side@base", "base" } },
		{ "Flex 2", { "amb@world_human_muscle_flex@arms_at_side@idle_a", "idle_a" } },
		{ "Flex 3", { "amb@world_human_muscle_flex@arms_in_front@base", "base" } },
		{ "Flex 4", { "amb@world_human_muscle_flex@arms_in_front@idle_a", "idle_b" } }
	};
	inline constexpr animation g_sexAnimations[8]{
		{ "Fuck 1", { "rcmpaparazzo_2", "shag_loop_a" } },
		{ "Fuck 2", { "rcmpaparazzo_2", "shag_loop_poppy" } },
		{ "Prostitue Sex (Male)", { "mini@prostitutes@sexnorm_veh", "sex_loop_male" } },
		{ "Prostitue Sex (Female)", { "mini@prostitutes@sexnorm_veh", "sex_loop_prostitute" } },
		{ "Prostitue BJ (Male)", { "mini@prostitutes@sexnorm_veh", "bj_loop_male" } },
		{ "Prostitue BJ (Female)", { "mini@prostitutes@sexnorm_veh", "bj_loop_prostitute" } },
		{ "Drunk Sex (Male)", { "random@drunk_driver_2", "cardrunksex_loop_m" } },
		{ "Drunk Sex (Female)", { "random@drunk_driver_2", "cardrunksex_loop_f" } }
	};
	inline constexpr animation g_danceAnimations[8]{
		{ "Pole Dance 1", { "mini@strip_club@pole_dance@pole_dance1", "pd_dance_01" } },
		{ "Pole Dance 2", { "mini@strip_club@pole_dance@pole_dance2", "pd_dance_02" } },
		{ "Pole Dance 3", { "mini@strip_club@pole_dance@pole_dance3", "pd_dance_03" } },
		{ "Private Dance", { "mini@strip_club@private_dance@part1", "priv_dance_p1" } },
		{ "Tap Dance", { "special_ped@mountain_dancer@monologue_2@monologue_2a", "mnt_dnc_angel" } },
		{ "Buttwag Dance", { "special_ped@mountain_dancer@monologue_3@monologue_3a", "mnt_dnc_buttwag" } },
		{ "Verse Dance", { "special_ped@mountain_dancer@monologue_4@monologue_4a", "mnt_dnc_verse" } },
		{ "Heaven Dance", { "special_ped@mountain_dancer@monologue_1@monologue_1a", "mtn_dnc_if_you_want_to_get_to_heaven" } }
	};
	inline constexpr animation g_superHeroAnimations[14]{
		{ "I'm An Actor", { "special_ped@impotent_rage@convo_1@convo_1a", "im_an_actor_0" } },
		{ "You Can't Stop Me", { "special_ped@impotent_rage@monologue_12@monologue_12d", "you_cant_stop_my_3" } },
		{ "I Wanna Do Hamlet", { "special_ped@impotent_rage@convo_2@convo_2a", "i_wanna_do_hamlet_0" } },
		{ "Having About As Much", { "special_ped@impotent_rage@convo_3@convo_3a", "having_about_as_much_0" } },
		{ "Just Wondering When", { "special_ped@impotent_rage@convo_4@convo_4a", "just_woundering_when_0" } },
		{ "Great I Wanted To", { "special_ped@impotent_rage@convo_5@convo_5a", "great_i_wanted_to_0" } },
		{ "A Tourist Just Asked to", { "special_ped@impotent_rage@convo_6@convo_6a", "a_tourist_just_asked_0" } },
		{ "Idle Intro", { "special_ped@impotent_rage@intro", "idle_intro" } },
		{ "Defeat 1", { "special_ped@impotent_rage@monologue_10@monologue_10b", "i_will_defeat_the_1" } },
		{ "Defeat 2", { "special_ped@impotent_rage@monologue_10@monologue_10c", "i_will_defeat_the_2" } },
		{ "Unhappy 1", { "special_ped@impotent_rage@monologue_11@monologue_11a", "really_very_unhappy_0" } },
		{ "Unhappy 2", { "special_ped@impotent_rage@monologue_11@monologue_11b", "really_very_unhappy_1" } },
		{ "Unhappy 3", { "special_ped@impotent_rage@monologue_11@monologue_11c", "really_very_unhappy_2" } },
		{ "Unhappy 4", { "special_ped@impotent_rage@monologue_11@monologue_11d", "really_very_unhappy_3" } }
	};
	inline constexpr animation g_miscellaneousAnimations[10]{
		{ "Tazer", { "ragdoll@human", "electrocute" } },
		{ "Stun Fire", { "ragdoll@human", "on_fire" } },
		{ "Electrocute", { "stungun@standing", "damage" } },
		{ "Waving Arms", { "random@car_thief@victimpoints_ig_3", "arms_waving" } },
		{ "Meditating", { "rcmcollect_paperleadinout@", "meditiate_idle" } },
		{ "Rolling Barrel", { "misschinese2_barrelroll", "barrel_roll_loop_A" } },
		{ "Cop Kneeling Arrest", { "random@arrests", "kneeling_arrest_idle" } },
		{ "Jerking Off", { "switch@trevor@jerking_off", "trev_jerking_off_loop" } },
		{ "Drunk Howling", { "switch@trevor@drunk_howling", "loop" } },
		{ "Pee", { "misscarsteal2peeing", "peeing_intro" } }
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