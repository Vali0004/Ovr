#pragma once
#include "includes.h"
#include "util/transaction.h"

namespace tabs::recovery {
	inline void tab() {
		elements::menu("Recovery", [] {
			elements::button("Impulse detecc", [] {
				util::transactions::basic("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 0x129A5B6E, 0x562592BB, 50000, 2);
			});
			elements::button("Test", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_JOB_BONUS"_joaat),
						1, 
						15000000,
						0,
						1
					}
				});
			});

			elements::button("SERVICE_EARN_ASSASSINATE_TARGET_KILLED", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_ASSASSINATE_TARGET_KILLED"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("CATEGORY_SERVICE_WITH_THRESHOLD", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_ARENA_WAR"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_REFUND_ARENA_SPEC_BOX_ENTRY", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_REFUND_ARENA_SPEC_BOX_ENTRY"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AMBIENT_JOB_RC_TIME_TRIAL", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AMBIENT_JOB_RC_TIME_TRIAL"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_DAILY_OBJECTIVE_EVENT", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_DAILY_OBJECTIVE_EVENT"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_COLLECTABLES_ACTION_FIGURES", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_COLLECTABLES_ACTION_FIGURES"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_MISSION_REWARD", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_MISSION_REWARD"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_AWARD_MISSION_ONE_FIRST_TIME", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_AWARD_MISSION_ONE_FIRST_TIME"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_AWARD_MISSION_TWO_FIRST_TIME", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_AWARD_MISSION_TWO_FIRST_TIME"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_AWARD_MISSION_THREE_FIRST_TIME", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_AWARD_MISSION_THREE_FIRST_TIME"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_AWARD_MISSION_FOUR_FIRST_TIME", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_AWARD_MISSION_FOUR_FIRST_TIME"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_AWARD_MISSION_FIVE_FIRST_TIME", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_AWARD_MISSION_FIVE_FIRST_TIME"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_AWARD_MISSION_SIX_FIRST_TIME", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_AWARD_MISSION_SIX_FIRST_TIME"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_AWARD_STRAIGHT_FLUSH", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_AWARD_STRAIGHT_FLUSH"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_AWARD_TOP_PAIR", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_AWARD_TOP_PAIR"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_AWARD_FULL_HOUSE", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_AWARD_FULL_HOUSE"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_AWARD_LUCKY_LUCKY", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_AWARD_LUCKY_LUCKY"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_AWARD_HIGH_ROLLER_BRONZE", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_AWARD_HIGH_ROLLER_BRONZE"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_AWARD_HIGH_ROLLER_SILVER", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_AWARD_HIGH_ROLLER_SILVER"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_AWARD_HIGH_ROLLER_GOLD", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_AWARD_HIGH_ROLLER_GOLD"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_AWARD_HIGH_ROLLER_PLATINUM", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_AWARD_HIGH_ROLLER_PLATINUM"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_STORY_MISSION_REWARD", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_STORY_MISSION_REWARD"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_HEIST_SETUP_MISSION", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_HEIST_SETUP_MISSION"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_HEIST_PREP_MISSION", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_HEIST_PREP_MISSION"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_HEIST_FINALE", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_HEIST_FINALE"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_HEIST_AWARD_SMASH_N_GRAB", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_HEIST_AWARD_SMASH_N_GRAB"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_HEIST_AWARD_IN_PLAIN_SIGHT", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_HEIST_AWARD_IN_PLAIN_SIGHT"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_HEIST_AWARD_UNDETECTED", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_HEIST_AWARD_UNDETECTED"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_HEIST_AWARD_ALL_ROUNDER", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_HEIST_AWARD_ALL_ROUNDER"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_HEIST_AWARD_ELITE_THIEF", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_HEIST_AWARD_ELITE_THIEF"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_HEIST_AWARD_PROFESSIONAL", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_HEIST_AWARD_PROFESSIONAL"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_HEIST_ELITE_STEALTH", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_HEIST_ELITE_STEALTH"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_HEIST_ELITE_SUBTERFUGE", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_HEIST_ELITE_SUBTERFUGE"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_CASINO_HEIST_ELITE_DIRECT", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_CASINO_HEIST_ELITE_DIRECT"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("CATEGORY_SERVICE_WITH_THRESHOLD", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_COLLECTABLE_ITEM"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_COLLECTABLE_COMPLETED_COLLECTION", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_COLLECTABLE_COMPLETED_COLLECTION"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_COLLECTABLES_SIGNAL_JAMMERS", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_COLLECTABLES_SIGNAL_JAMMERS"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_COLLECTABLES_SIGNAL_JAMMERS_COMPLETE", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_COLLECTABLES_SIGNAL_JAMMERS_COMPLETE"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_ISLAND_HEIST_FINALE", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_ISLAND_HEIST_FINALE"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_ISLAND_HEIST_ELITE_CHALLENGE", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_ISLAND_HEIST_ELITE_CHALLENGE"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_ISLAND_HEIST_AWARD_PROFESSIONAL", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_ISLAND_HEIST_AWARD_PROFESSIONAL"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_ISLAND_HEIST_AWARD_ELITE_THIEF", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_ISLAND_HEIST_AWARD_ELITE_THIEF"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_ISLAND_HEIST_AWARD_THE_ISLAND_HEIST", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_ISLAND_HEIST_AWARD_THE_ISLAND_HEIST"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_ISLAND_HEIST_AWARD_GOING_ALONE", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_ISLAND_HEIST_AWARD_GOING_ALONE"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_ISLAND_HEIST_AWARD_TEAM_WORK", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_ISLAND_HEIST_AWARD_TEAM_WORK"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_ISLAND_HEIST_AWARD_CAT_BURGLAR", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_ISLAND_HEIST_AWARD_CAT_BURGLAR"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_ISLAND_HEIST_AWARD_PRO_THIEF", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_ISLAND_HEIST_AWARD_PRO_THIEF"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_ISLAND_HEIST_AWARD_MIXING_IT_UP", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_ISLAND_HEIST_AWARD_MIXING_IT_UP"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_ISLAND_HEIST_PREP", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_ISLAND_HEIST_PREP"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_ISLAND_HEIST_DJ_MISSION", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_ISLAND_HEIST_DJ_MISSION"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_TUNER_ROBBERY_PREP", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_TUNER_ROBBERY_PREP"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_TUNER_ROBBERY_FINALE", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_TUNER_ROBBERY_FINALE"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_TUNER_CAR_CLUB_MEMBERSHIP", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_TUNER_CAR_CLUB_MEMBERSHIP"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_TUNER_DAILY_VEHICLE", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_TUNER_DAILY_VEHICLE"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_TUNER_DAILY_VEHICLE_BONUS", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_TUNER_DAILY_VEHICLE_BONUS"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_TUNER_AWARD_UNION_DEPOSITORY", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_TUNER_AWARD_UNION_DEPOSITORY"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_TUNER_AWARD_MILITARY_CONVOY", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_TUNER_AWARD_MILITARY_CONVOY"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_TUNER_AWARD_FLEECA_BANK", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_TUNER_AWARD_FLEECA_BANK"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_TUNER_AWARD_FREIGHT_TRAIN", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_TUNER_AWARD_FREIGHT_TRAIN"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_TUNER_AWARD_BOLINGBROKE_ASS", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_TUNER_AWARD_BOLINGBROKE_ASS"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_TUNER_AWARD_IAA_RAID", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_TUNER_AWARD_IAA_RAID"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_TUNER_AWARD_METH_JOB", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_TUNER_AWARD_METH_JOB"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_TUNER_AWARD_BUNKER_RAID", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_TUNER_AWARD_BUNKER_RAID"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AUTO_SHOP_DELIVERY_AWARD", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AUTO_SHOP_DELIVERY_AWARD"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AGENCY_SECURITY_CONTRACT", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AGENCY_SECURITY_CONTRACT"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AGENCY_PAYPHONE_HIT", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AGENCY_PAYPHONE_HIT"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AGENCY_STORY_PREP", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AGENCY_STORY_PREP"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AGENCY_STORY_FINALE", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AGENCY_STORY_FINALE"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_FIXER_AWARD_SEC_CON", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_FIXER_AWARD_SEC_CON"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_FIXER_AWARD_PHONE_HIT", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_FIXER_AWARD_PHONE_HIT"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_FIXER_AWARD_AGENCY_STORY", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_FIXER_AWARD_AGENCY_STORY"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_FIXER_AWARD_SHORT_TRIP", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_FIXER_AWARD_SHORT_TRIP"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_FIXER_RIVAL_DELIVERY", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_FIXER_RIVAL_DELIVERY"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_MUSIC_STUDIO_SHORT_TRIP", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_MUSIC_STUDIO_SHORT_TRIP"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_FROM_CONTRABAND", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_FROM_CONTRABAND"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_NCLUB_TROUBLEMAKER", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_NCLUB_TROUBLEMAKER"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_SIGHTSEEING_REWARD", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_SIGHTSEEING_REWARD"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AMBIENT_JOB_CLUBHOUSE_CONTRACT", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AMBIENT_JOB_CLUBHOUSE_CONTRACT"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AMBIENT_JOB_UNDERWATER_CARGO", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AMBIENT_JOB_UNDERWATER_CARGO"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AMBIENT_JOB_CRIME_SCENE", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AMBIENT_JOB_CRIME_SCENE"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AMBIENT_JOB_METAL_DETECTOR", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AMBIENT_JOB_METAL_DETECTOR"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AMBIENT_JOB_SMUGGLER_PLANE", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AMBIENT_JOB_SMUGGLER_PLANE"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AMBIENT_JOB_SMUGGLER_TRAIL", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AMBIENT_JOB_SMUGGLER_TRAIL"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AMBIENT_JOB_GOLDEN_GUN", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AMBIENT_JOB_GOLDEN_GUN"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AMBIENT_JOB_AMMUNATION_DELIVERY", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AMBIENT_JOB_AMMUNATION_DELIVERY"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_AMBIENT_JOB_SOURCE_RESEARCH", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_AMBIENT_JOB_SOURCE_RESEARCH"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});

			elements::button("SERVICE_EARN_YOHAN_SOURCE_GOODS", [] {
				util::transactions::basket("CATEGORY_SERVICE_WITH_THRESHOLD"_joaat, 1445302971, 4, {
					{
						static_cast<i32>("SERVICE_EARN_YOHAN_SOURCE_GOODS"_joaat),
						1,
						15000000,
						0,
						1
					}
					});
				});
		});
	}
}
/*
* found in tattoo_shop.c line 68394  
// func_522(uParam2, -1135378931, 1445302971, joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), hParam0, iParam1, num, 7);

//int func_586(int iParam0, int iParam1, int iParam2) // Position - 0x5D2BC
*/