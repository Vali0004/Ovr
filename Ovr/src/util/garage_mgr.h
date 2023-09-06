#pragma once
#include "pch/pch.h"
#include "util/global.h"
#include "util/util.h"
#include "rage/commands/list.h"

namespace util {
	inline const char* g_garageLabels[]{ //am_mp_property_ext.c, Hash func_5510(int iParam0) (MP_STAT and _v0 is removed, as it isn't needed)
		"PROPERTY_HOUSE",
		"MULTI_PROPERTY_1",
		"MULTI_PROPERTY_2",
		"MULTI_PROPERTY_3",
		"MULTI_PROPERTY_4",
		"PROP_OFFICE",
		"PROP_CLUBHOUSE",
		"MULTI_PROPERTY_5",
		"PROP_OFFICE_GAR1",
		"PROP_OFFICE_GAR2",
		"PROP_OFFICE_GAR3",
		"PROP_IE_WAREHOUSE",
		"PROP_HANGAR",
		"PROP_DEFUNCBASE",
		"PROP_NIGHTCLUB",
		"PROP_MEGAWARE_GAR1",
		"PROP_MEGAWARE_GAR2",
		"PROP_MEGAWARE_GAR3",
		"PROP_ARENAWARS_GAR1",
		"PROP_ARENAWARS_GAR2",
		"PROP_ARENAWARS_GAR3",
		"PROP_CASINO_GAR1",
		"PROP_ARCADE_GAR1",
		"MULTI_PROPERTY_6",
		"MULTI_PROPERTY_7",
		"MULTI_PROPERTY_8",
		"MULTI_PROPERTY_9"
	};
	template <typename t>
	t stat_get(const char* stat_name) {
		t value;
		bool use_character{ strstr(stat_name, "MPPLY") == NULL && strstr(stat_name, "MP_PLAYING_TIM") == NULL };
		char stat[100]{};
		sprintf_s(stat, "%s%s", use_character ? ("MP" + std::to_string(0) + "_").c_str() : "", stat_name);
		if (std::is_same<t, int>::value) {
			STATS::STAT_GET_INT(MISC::GET_HASH_KEY(stat), (int*)&value, true);
		}
		else if (std::is_same<t, float>::value) {
			STATS::STAT_GET_FLOAT(MISC::GET_HASH_KEY(stat), (float*)&value, true);
		}
		else if (std::is_same<t, bool>::value) {
			STATS::STAT_GET_BOOL(MISC::GET_HASH_KEY(stat), (BOOL*)&value, true);
		}
		return value;
	}
	namespace rage_ysc {
		//am_dead_drop.c
		#define _INVALID_PLAYER_INDEX() -1
		inline global propertyBase2{ 2657704 };
		inline uint64_t propertyBase2Size{ 463 };
		inline uint64_t propertyBase2Flag{ 199 };
		inline global propertyBase3{ 1853822 };
		inline global propertyBase4{ 1312228 };
		inline uint64_t propertyBase4Size{ 1951 };
		inline uint64_t propertyBase4Flag{ 33 };
		inline global playerStats{ 1853988 };
		inline uint64_t playerStatsSize{ 867 };
		inline uint64_t playerPropertStat{ 267 };
		inline uint64_t playerPropertStat2{ 287 };
		//arena_carmod.c
		inline uint64_t playerPropertStat3{ 300 };
		inline uint64_t playerPropertStat4{ 293 };
		inline global tunables{ 262145 };
		inline uint64_t propertyTunablesOffset{ 10802 };
		inline global propertyBase5{ 1945123 };
		inline global garage{ 1586488 }; //Search "DEL_VEH_SEL3" and scroll down until you see "STREAMING::IS_MODEL_IN_CDIMAGE". See that arg
		inline uint64_t garageSize{ 142 };
		inline global gargaeModsUnk{ 152699 };
		//custom native reimpl
		inline BOOL IS_BIT_SET(int address, int offset) {
			bool result = false;
			if (offset < 32) {
				result = (address & (1 << offset)) != 0;
			}
			return result;
		}
		inline BOOL func_34(int iParam0) {
			switch (iParam0) {
				case 87:
				case 88:
				case 89:
				case 90:
					return true;
			}
			return false;
		}
		inline int func_601(int iParam0) {
			switch (iParam0) {
				case 0:
					return 157;
				case 2:
					return 224;
				case 1:
					return 227;
				case 3:
					return 279;
				default:
					return -1;
			}
		}
		inline int func_602(int iParam0) {
			iParam0 = iParam0 - 1000;
			if (iParam0 >= 0 && iParam0 <= 4)
				return iParam0;
			return -1;
		}
		inline BOOL func_599(Player plParam0) {
			if (plParam0 != _INVALID_PLAYER_INDEX())
				return IS_BIT_SET(propertyBase2.at(plParam0, propertyBase2Size).at(propertyBase2Flag).value()->Int, 9);

			return false;
		}
		inline BOOL func_600(Player plParam0) {
			if (plParam0 != _INVALID_PLAYER_INDEX())
				return IS_BIT_SET(playerStats.at(plParam0, playerStatsSize).at(playerPropertStat).at(playerPropertStat2).value()->Int, 2);

			return false;
		}
		inline BOOL func_598(Player plParam0, BOOL bParam1) {
			if (propertyBase3.value()->Int != _INVALID_PLAYER_INDEX()) {
				if (!func_600(propertyBase3.value()->Int))
					return false;

				if (bParam1) {
					if (PLAYER::PLAYER_ID() != propertyBase3.value()->Int) {
						if (IS_BIT_SET(propertyBase2.at(propertyBase3.value()->Int, propertyBase2Size).at(propertyBase2Flag).value()->Int, 24) || func_599(propertyBase3.value()->Int)) {
							return true;
						}
					}
				}
			}

			return IS_BIT_SET(propertyBase2.at(plParam0, propertyBase2Size).at(propertyBase2Flag).value()->Int, 24);
		}
		inline BOOL func_596(int iParam0, int iParam1) {
			if (iParam1 == -1) {
				switch (iParam0) {
					case 91:
					case 92:
					case 93:
					case 94:
					case 95:
					case 96:
					case 97:
					case 98:
					case 99:
					case 100:
					case 101:
					case 102:
						return true;
				}
			}
			else if (iParam1 == 91) {
				switch (iParam0) {
					case 91:
					case 92:
					case 93:
					case 94:
					case 95:
					case 96:
						return true;
				}
			}
			else if (iParam1 == 97) {
				switch (iParam0) {
					case 97:
					case 98:
					case 99:
					case 100:
					case 101:
					case 102:
						return true;
				}
			}
			return false;
		}
		inline BOOL func_597(int iParam0, BOOL bParam1, BOOL bParam2) {
			if (bParam2)
				return func_598(PLAYER::PLAYER_ID(), false);
			if (bParam1) {
				if (func_598(PLAYER::PLAYER_ID(), false))
					return false;
				switch (iParam0) {
					case 103:
					case 106:
					case 109:
					case 112:
					case 104:
					case 107:
					case 110:
					case 113:
					case 105:
					case 108:
					case 111:
					case 114:
						return true;
				}
			}
			switch (iParam0) {
				case 103:
				case 106:
				case 109:
				case 112:
				case 104:
				case 107:
				case 110:
				case 113:
				case 105:
				case 108:
				case 111:
				case 114:
					return true;
			}
			return false;
		}
		inline int get_garage_vehicle_max_count(int iParam0, int iParam1, BOOL bParam2) {
			if (iParam0 == -1) {
				if (iParam1 >= 1) {
					if (func_597(iParam1, false, false))
						return 20;
					else if (func_34(iParam1))
						return 0;
					else if (func_596(iParam1, -1))
						return 10;
					else if (iParam1 == 115)
						return 8;
					else if (iParam1 == 116)
						return 20;
					else if (iParam1 == 117)
						return 7;
					else if (iParam1 == 118)
						return 1;
					else if (iParam1 == 119 || iParam1 == 120 || iParam1 == 121)
						return 10;
					else if (iParam1 == 122)
						return 9;
					else if (iParam1 == 123 || iParam1 == 124)
						return 10;
					else if (iParam1 == 125)
						return 10;
					else if (iParam1 == 126)
						return 10;
					else if (iParam1 == 127)
						return 10;
					else if (iParam1 == 128)
						return 20;
					else if (iParam1 == 129)
						return 50;
					else if (iParam1 <= 130 && iParam1 > 0) {
						if (propertyBase4.at(iParam1, propertyBase4Size).at(propertyBase4Flag).value()->Int == 2) {
							if (bParam2)
								return 3;
							else
								return 2;
						}
						else if (propertyBase4.at(iParam1, propertyBase4Size).at(propertyBase4Flag).value()->Int == 6) {
							if (bParam2)
								return 8;
							else
								return 6;
						}
						else if (propertyBase4.at(iParam1, propertyBase4Size).at(propertyBase4Flag).value()->Int == 10) {
							if (bParam2)
								return 13;
							else
								return 10;
						}
					}
				}
			}
			switch (iParam0) {
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 7:
				case 23:
				case 24:
				case 27:
				case 28:
					return 13;
				case 5:
					return 0;
				case 6:
					return 10;
				case 8:
				case 9:
				case 10:
					return 20;
				case 11:
					return 8;
				case 12:
					return 20;
				case 13:
					return 7;
				case 14:
					return 1;
				case 15:
				case 16:
				case 17:
					return 10;
				case 18:
				case 19:
				case 20:
					return 10;
				case 21:
					return 10;
				case 22:
					return 10;
				case 25:
					return 10;
				case 26:
					return 20;
				case 29:
					return 50;
			}
			return 0;
		}
		inline int func_594(int iParam0) {
			switch (iParam0) {
				case 8:
					return 108;
				case 9:
					return 128;
				case 10:
					return 148;
				case 11:
					return 156;
				case 6:
					return 75;
				case 7:
					return 88;
				case 5:
					return -1;
				case 12:
					return 179;
				case 13:
					return 186;
				case 14:
					return 192;
				case 15:
					return 202;
				case 16:
					return 212;
				case 17:
					return 222;
				case 18:
					return 236;
				case 19:
					return 246;
				case 20:
					return 256;
				case 21:
					return 268;
				case 22:
					return 278;
				case 23:
					return 294;
				case 24:
					return 307;
				case 25:
					return 317;
				case 26:
					return 337;
				case 27:
					return 350;
				case 28:
					return 363;
				case 29:
					return 413;
			}
			if (iParam0 >= 1000) {
				int num{ func_602(iParam0) };
				return func_601(num);
			}
			return get_garage_vehicle_max_count(iParam0, -1, true) * (iParam0 + 1);
		}
		//am_mp_arena_garage.c
		inline void func_210(int iParam0, int* iParam1, BOOL bParam2) {
			if (tunables.at(propertyTunablesOffset).value()->Int)
				*iParam1 = iParam0;
			if (iParam0 >= 0) {
				*iParam1 = propertyBase5.at(iParam0).value()->Int - 1;
			}
			else {
				*iParam1 = -1;
			}
			return;
		}
		//arena_carmod.c
		inline int func_1081(Player plParam0) {
			if (plParam0 != _INVALID_PLAYER_INDEX())
				return playerStats.at(plParam0, playerStatsSize).at(playerPropertStat).at(playerPropertStat3).value()->Int;
			return 0;
		}
		inline int func_1083(Player plParam0) {
			if (plParam0 != _INVALID_PLAYER_INDEX())
				return playerStats.at(plParam0, playerStatsSize).at(playerPropertStat).at(playerPropertStat4).value()->Int;
			return 0;
		}
		inline const char* func_2411(int iParam0) {
			switch (iParam0) {
				case 1:
					return "MP_DBASE_1" /*Grand Senora Desert Facility*/;
				case 2:
					return "MP_DBASE_2" /*Route 68 Facility*/;
				case 3:
					return "MP_DBASE_3" /*Sandy Shores Facility*/;
				case 4:
					return "MP_DBASE_4" /*Mount Gordo Facility*/;
				case 5:
					return "MP_DBASE_6" /*Paleto Bay Facility*/;
				case 6:
					return "MP_DBASE_7" /*Lago Zancudo Facility*/;
				case 7:
					return "MP_DBASE_8" /*Zancudo River Facility*/;
				case 8:
					return "MP_DBASE_9" /*Ron Alternates Wind Farm Facility*/;
				case 9:
					return "MP_DBASE_10" /*Land Act Reservoir Facility*/;
			}
			return "";
		}
		inline const char* func_2412(int iParam0) {
			switch (iParam0) {
				case 1:
					return "MP_HANGAR_1" /*LSIA Hangar 1*/;
				case 2:
					return "MP_HANGAR_2" /*LSIA Hangar A17*/;
				case 3:
					return "MP_HANGAR_3" /*Fort Zancudo Hangar A2*/;
				case 4:
					return "MP_HANGAR_4" /*Fort Zancudo Hangar 3497*/;
				case 5:
					return "MP_HANGAR_5" /*Fort Zancudo Hangar 3499*/;
			}
			return "";
		}
		//am_mp_property_int.c
		inline int func_1287(int iParam0) {
			switch (iParam0) {
				case 0:
					return 156;
				case 2:
					return 223;
				case 1:
					return 224;
				case 3:
					return 278;
			}
			return -1;
		}
		inline int func_8611(int iParam0) {
			int num;
			switch (iParam0) {
				case 8:
					return 88;
				case 9:
					return 108;
				case 10:
					return 128;
				case 11:
					return 148;
				case 6:
					return 65;
				case 7:
					return 75;
				case 5:
					return -1;
				case 12:
					return 159;
				case 13:
					return 179;
				case 14:
					return 191;
				case 15:
					return 192;
				case 16:
					return 202;
				case 17:
					return 212;
				case 18:
					return 227;
				case 19:
					return 237;
				case 20:
					return 247;
				case 21:
					return 258;
				case 22:
					return 268;
				case 23:
					return 281;
				case 24:
					return 294;
				case 25:
					return 307;
				case 26:
					return 317;
				case 27:
					return 337;
				case 28:
					return 350;
				case 29:
					return 363;
			}
			if (iParam0 >= 1000) {
				num = func_602(iParam0);
				return func_1287(num);
			}
			return get_garage_vehicle_max_count(iParam0, -1, true) * iParam0;
		}
		inline const char* get_location_from_id(int id) {
			const char* value = "";
			switch (id) {
				case 0:
					value = "MP_REP_PROP_1";
					break;
				case 1:
					value = "MP_PROP_1";
					break;
				case 2:
					value = "MP_PROP_2";
					break;
				case 3:
					value = "MP_PROP_3";
					break;
				case 4:
					value = "MP_PROP_4";
					break;
				case 5:
					value = "MP_PROP_5";
					break;
				case 6:
					value = "MP_PROP_6";
					break;
				case 7:
					value = "MP_PROP_7";
					break;
				case 8:
					value = "MP_PROP_8";
					break;
				case 9:
					value = "MP_PROP_9";
					break;
				case 10:
					value = "MP_PROP_10";
					break;
				case 11:
					value = "MP_PROP_11";
					break;
				case 12:
					value = "MP_PROP_12";
					break;
				case 13:
					value = "MP_PROP_13";
					break;
				case 14:
					value = "MP_PROP_14";
					break;
				case 15:
					value = "MP_PROP_15";
					break;
				case 16:
					value = "MP_PROP_16";
					break;
				case 17:
					value = "MP_PROP_17";
					break;
				case 18:
					value = "MP_PROP_18";
					break;
				case 19:
					value = "MP_PROP_19";
					break;
				case 20:
					value = "MP_PROP_20";
					break;
				case 21:
					value = "MP_PROP_21";
					break;
				case 22:
					value = "MP_PROP_22";
					break;
				case 23:
					value = "MP_PROP_23";
					break;
				case 24:
					value = "MP_PROP_24";
					break;
				case 25:
					value = "MP_PROP_25";
					break;
				case 26:
					value = "MP_PROP_26";
					break;
				case 27:
					value = "MP_PROP_27";
					break;
				case 28:
					value = "MP_PROP_28";
					break;
				case 29:
					value = "MP_PROP_29";
					break;
				case 30:
					value = "MP_PROP_30";
					break;
				case 31:
					value = "MP_PROP_31";
					break;
				case 32:
					value = "MP_PROP_32";
					break;
				case 33:
					value = "MP_PROP_33";
					break;
				case 34:
					value = "MP_PROP_34";
					break;
				case 35:
					value = "MP_PROP_35";
					break;
				case 36:
					value = "MP_PROP_36";
					break;
				case 37:
					value = "MP_PROP_37";
					break;
				case 38:
					value = "MP_PROP_38";
					break;
				case 39:
					value = "MP_PROP_39";
					break;
				case 40:
					value = "MP_PROP_40";
					break;
				case 41:
					value = "MP_PROP_41";
					break;
				case 42:
					value = "MP_PROP_42";
					break;
				case 43:
					value = "MP_PROP_43";
					break;
				case 44:
					value = "MP_PROP_44";
					break;
				case 45:
					value = "MP_PROP_45";
					break;
				case 46:
					value = "MP_PROP_46";
					break;
				case 47:
					value = "MP_PROP_48";
					break;
				case 48:
					value = "MP_PROP_49";
					break;
				case 49:
					value = "MP_PROP_50";
					break;
				case 50:
					value = "MP_PROP_51";
					break;
				case 51:
					value = "MP_PROP_52";
					break;
				case 52:
					value = "MP_PROP_57";
					break;
				case 53:
					value = "MP_PROP_59";
					break;
				case 54:
					value = "MP_PROP_60";
					break;
				case 55:
					value = "MP_PROP_61";
					break;
				case 56:
					value = "MP_PROP_62";
					break;
				case 57:
					value = "MP_PROP_63";
					break;
				case 58:
					value = "MP_PROP_64";
					break;
				case 59:
					value = "MP_PROP_65";
					break;
				case 60:
					value = "MP_PROP_66";
					break;
				case 61:
					value = "MP_PROP_67";
					break;
				case 62:
					value = "MP_PROP_68";
					break;
				case 63:
					value = "MP_PROP_69";
					break;
				case 64:
					value = "MP_PROP_70";
					break;
				case 65:
					value = "MP_PROP_71";
					break;
				case 66:
					value = "MP_PROP_72";
					break;
				case 67:
					value = "MP_PROP_73";
					break;
				case 68:
					value = "MP_PROP_74";
					break;
				case 69:
					value = "MP_PROP_75";
					break;
				case 70:
					value = "MP_PROP_76";
					break;
				case 71:
					value = "MP_PROP_77";
					break;
				case 72:
					value = "MP_PROP_78";
					break;
				case 83:
					value = "MP_PROP_79";
					break;
				case 84:
					value = "MP_PROP_80";
					break;
				case 85:
					value = "MP_PROP_81";
					break;
				case 73:
					value = "MP_PROP_83";
					break;
				case 74:
					value = "MP_PROP_84";
					break;
				case 75:
					value = "MP_PROP_85";
					break;
				case 76:
					value = "MP_PROP_86";
					break;
				case 77:
					value = "MP_PROP_87";
					break;
				case 78:
					value = "MP_PROP_89";
					break;
				case 79:
					value = "MP_PROP_90";
					break;
				case 80:
					value = "MP_PROP_92";
					break;
				case 81:
					value = "MP_PROP_94";
					break;
				case 82:
					value = "MP_PROP_95";
					break;
				case 86:
					value = "PM_SPAWN_Y";
					break;
				case 87:
					value = "MP_PROP_OFF1";
					break;
				case 88:
					value = "MP_PROP_OFF2";
					break;
				case 89:
					value = "MP_PROP_OFF3";
					break;
				case 90:
					value = "MP_PROP_OFF4";
					break;
				case 91:
					value = "MP_PROP_CLUBH1";
					break;
				case 92:
					value = "MP_PROP_CLUBH2";
					break;
				case 93:
					value = "MP_PROP_CLUBH3";
					break;
				case 94:
					value = "MP_PROP_CLUBH4";
					break;
				case 95:
					value = "MP_PROP_CLUBH5";
					break;
				case 96:
					value = "MP_PROP_CLUBH6";
					break;
				case 97:
					value = "MP_PROP_CLUBH7";
					break;
				case 98:
					value = "MP_PROP_CLUBH8";
					break;
				case 99:
					value = "MP_PROP_CLUBH9";
					break;
				case 100:
					value = "MP_PROP_CLUBH10";
					break;
				case 101:
					value = "MP_PROP_CLUBH11";
					break;
				case 102:
					value = "MP_PROP_CLUBH12";
					break;
				case 103:
				case 106:
				case 109:
				case 112:
					value = "MP_PROP_OFFG1";
					break;
				case 104:
				case 107:
				case 110:
				case 113:
					value = "MP_PROP_OFFG2";
					break;
				case 105:
				case 108:
				case 111:
				case 114:
					value = "MP_PROP_OFFG3";
					break;
				case 115:
					value = "IE_WARE_1";
					break;
			}
			return HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(value);
		}
		//am_mp_garage_control.c
		BOOL func_479(Vehicle veParam0) {
			Hash hashKey{};
			if (ENTITY::DOES_ENTITY_EXIST(veParam0) && VEHICLE::IS_VEHICLE_DRIVEABLE(veParam0, false) && VEHICLE::GET_NUM_MOD_KITS(veParam0) > 0) {
				VEHICLE::SET_VEHICLE_MOD_KIT(veParam0, 0);
				for (int modType{}; modType != 49; ++modType) {
					if (modType == 17 || modType == 18 || modType == 19 || modType == 20 || modType == 21 || modType == 22) {
					}
					else if (VEHICLE::GET_VEHICLE_MOD(veParam0, modType) != -1) {
						const char* label{ VEHICLE::GET_MOD_TEXT_LABEL(veParam0, modType, VEHICLE::GET_VEHICLE_MOD(veParam0, modType)) };
						hashKey = MISC::GET_HASH_KEY(label);

						if (hashKey != 0)
							if (hashKey == MISC::GET_HASH_KEY("MNU_CAGE" /*Roll Cage and Chassis Upgrade*/) || hashKey == MISC::GET_HASH_KEY("SABRE_CAG" /*Stunt Cage*/))
								return true;
					}
				}
			}
			return false;
		}
		void func_480(Vehicle& vehicle) {
			switch (ENTITY::GET_ENTITY_MODEL(vehicle)) {
				case "starling"_joaat:
					if (VEHICLE::GET_VEHICLE_MOD(vehicle, 4) == 0)
						VEHICLE::SET_VEHICLE_MOD(vehicle, eVehicleMod::VehicleModTransmission, 0, false);
					else
						VEHICLE::REMOVE_VEHICLE_MOD(vehicle, eVehicleMod::VehicleModTransmission);
					break;
				case "slamtruck"_joaat:
					VEHICLE::SET_VEHICLE_MOD(vehicle, eVehicleMod::VehicleModDialDesign, 3, false);
					break;
				default:
					break;
			}
			return;
		}
		int func_482(Vehicle vehicle, int modType) {
			return VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, modType);
		}
		int func_481(Vehicle vehicle, int iParam1) {
			int value{}, value2{};
			int num2{};
			if (ENTITY::DOES_ENTITY_EXIST(vehicle) && VEHICLE::IS_VEHICLE_DRIVEABLE(vehicle, false)) {
				switch (ENTITY::GET_ENTITY_MODEL(vehicle)) {
					case "tornado5"_joaat:
						switch (iParam1) {
							case 0:
								return 0;
							case 1:
								return 1;
							case 2:
								return 2;
							case 3:
								return 3;
							case 4:
								return 4;
							case 5:
								return 4;
						}
						break;
					case "faction3"_joaat:
						return 3;
				}
				value = func_482(vehicle, 38);
				value2 = func_482(vehicle, 24);
				num2 = float(static_cast<float>(value2) * (static_cast<float>(iParam1 + 1) / static_cast<float>(value))) - 1;
				if (num2 < 0)
					num2 = 0;
				if (num2 >= value)
					num2 = value - 1;
				return num2;
			}
			return 0;
		}
		BOOL func_483(Hash hParam0, int iParam1) {
			switch (hParam0) {
				case "faction2"_joaat:
				case "buccaneer2"_joaat:
				case "chino2"_joaat:
				case "moonbeam2"_joaat:
				case "primo2"_joaat:
				case "voodoo"_joaat:
					return true;
				case "tenf2"_joaat:
				case "weevil2"_joaat:
				case "brioso3"_joaat:
				case "sentinel4"_joaat:
					if ((iParam1 & 1) != 0)
						return false;
					return true;

				case "sabregt2"_joaat:
					if (!tunables.at(14909).value()->Int)
						return false;
					else
						return true;
					break;
				case "tornado5"_joaat:
					if (!tunables.at(14910).value()->Int)
						return false;
					else
						return true;
					break;
				case "virgo2"_joaat:
					if (!tunables.at(14908).value()->Int)
						return false;
					else
						return true;
					break;
				case "minivan2"_joaat:
					if (!tunables.at(14911).value()->Int)
						return false;
					else
						return true;
					break;
				case "slamvan3"_joaat:
					if (!tunables.at(14913).value()->Int)
						return false;
					else
						return true;
					break;
				case "faction3"_joaat:
					if (!tunables.at(14912).value()->Int)
						return false;
					else
						return true;
					break;
				case "sultanrs"_joaat:
				case "banshee2"_joaat:
					if ((iParam1 & 1) != 0)
						return false;
					return true;
				case "comet3"_joaat:
					if (tunables.at(19321).value()->Int) {
						if ((iParam1 & 1) != 0)
							return false;
						return true;
					}
					return false;
				case "diablous2"_joaat:
					if (tunables.at(19323).value()->Int) {
						if ((iParam1 & 1) != 0)
							return false;

						return true;
					}
					return false;
				case "fcr2"_joaat:
					if (tunables.at(19327).value()->Int) {
						if ((iParam1 & 1) != 0)
							return false;
						return true;
					}
					return false;
				case "elegy"_joaat:
					if (tunables.at(19324).value()->Int) {
						if ((iParam1 & 1) != 0)
							return false;
						return true;
					}
					return false;
				case "nero2"_joaat:
					if (tunables.at(19331).value()->Int) {
						if ((iParam1 & 1) != 0)
							return false;
						return true;
					}
					return false;
				case "italigtb2"_joaat:
					if (tunables.at(19329).value()->Int) {
						if ((iParam1 & 1) != 0)
							return false;
						return true;
					}
					return false;
				case "specter2"_joaat:
					if (tunables.at(19334).value()->Int) {
						if ((iParam1 & 1) != 0)
							return false;
						return true;
					}
					return false;
				case "technical3"_joaat:
					if (tunables.at(21280).value()->Int) {
						if ((iParam1 & 1) != 0)
							return false;
						return true;
					}
					return false;
				case "insurgent3"_joaat:
					if (tunables.at(2128).value()->Int) {
						if ((iParam1 & 1) != 0)
							return false;
						return true;
					}
					return false;
				case "slamvan4"_joaat:
				case "slamvan5"_joaat:
				case "slamvan6"_joaat:
					if ((iParam1 & 1) != 0)
						return false;
					return true;
				case "issi4"_joaat:
				case "issi5"_joaat:
				case "issi6"_joaat:
					if ((iParam1 & 1) != 0)
						return false;
					return true;
				case "impaler2"_joaat:
				case "impaler3"_joaat:
				case "impaler4"_joaat:
					if ((iParam1 & 1) != 0)
						return false;
					return true;
				case "deathbike"_joaat:
				case "deathbike2"_joaat:
				case "deathbike3"_joaat:
					if ((iParam1 & 1) != 0)
						return false;
					return true;
				case "monster3"_joaat:
				case "monster4"_joaat:
				case "monster5"_joaat:
					if ((iParam1 & 1) != 0)
						return false;
					return true;
				case "dominator4"_joaat:
				case "dominator5"_joaat:
				case "dominator6"_joaat:
					if ((iParam1 & 1) != 0)
						return false;
					return true;
				case "bruiser"_joaat:
				case "bruiser2"_joaat:
				case "bruiser3"_joaat:
					if ((iParam1 & 1) != 0)
						return false;
					return true;
				case "youga3"_joaat:
				case "gauntlet5"_joaat:
				case "yosemite3"_joaat:
					if ((iParam1 & 1) != 0)
						return false;
					return true;
				case "manana2"_joaat:
				case "peyote3"_joaat:
				case "glendale2"_joaat:
					return true;
			}
			return false;
		}
		BOOL func_485() {
			return false;
		}
		BOOL func_484(Vehicle veParam0, int iParam1, int iParam2) {
			if (!func_485() && VEHICLE::IS_VEHICLE_MOD_GEN9_EXCLUSIVE(veParam0, iParam1, iParam2))
				return true;
			return false;
		}
		int apply_vehicle_mods(Vehicle& vehicle, int slot) {
			if (!VEHICLE::IS_VEHICLE_DRIVEABLE(vehicle, false))
				return 0;
			if (VEHICLE::GET_NUM_MOD_KITS(vehicle) == 0)
				return 0;
			VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
			global offsetGarageGlobal{ rage_ysc::garage.at(slot, rage_ysc::garageSize).at(static_cast<i32>(eGlobalGarageIndices::ModCount)) };
			int modCount{ offsetGarageGlobal.value()->Int };
			for (int modType{}; modType != modCount; ++modType) {
				if (modType == 17 || modType == 18 || modType == 19 || modType == 20 || modType == 21) {
					VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, modType, offsetGarageGlobal.at(modType).value()->Int > 0);
				}
				else if (modType == 22) {
					if (offsetGarageGlobal.at(modType).value()->Int > 0) {
						VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, modType, true);
						if (offsetGarageGlobal.at(modType).value()->Int == 1)
							VEHICLE::SET_VEHICLE_XENON_LIGHT_COLOR_INDEX(vehicle, 255);
						else
							VEHICLE::SET_VEHICLE_XENON_LIGHT_COLOR_INDEX(vehicle, offsetGarageGlobal.at(modType).value()->Int - 2);
					}
					else {
						VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, modType, false);
					}
				}
				else if (VEHICLE::GET_VEHICLE_MOD(vehicle, modType) != offsetGarageGlobal.at(modType).value()->Int - 1) {
					VEHICLE::REMOVE_VEHICLE_MOD(vehicle, modType);
					if (offsetGarageGlobal.at(modType).value()->Int > 0) {
						if (modType == 23) {
							VEHICLE::SET_VEHICLE_MOD(vehicle, modType, offsetGarageGlobal.at(modType).value()->Int - 1, offsetGarageGlobal.at(0).value()->Int > 0);
						}
						else if (modType == 24) {
							VEHICLE::SET_VEHICLE_MOD(vehicle, modType, offsetGarageGlobal.at(modType).value()->Int - 1, offsetGarageGlobal.at(1).value()->Int > 0);
						}
						else if (func_484(vehicle, modType, offsetGarageGlobal.at(modType).value()->Int - 1)) {
						}
						else {
							VEHICLE::SET_VEHICLE_MOD(vehicle, modType, offsetGarageGlobal.at(modType).value()->Int - 1, false);
						}
					}
				}
			}
			if (func_483(ENTITY::GET_ENTITY_MODEL(vehicle), 1) && VEHICLE::GET_VEHICLE_MOD(vehicle, 24) != func_481(vehicle, offsetGarageGlobal.at(38).value()->Int - 1))
				VEHICLE::SET_VEHICLE_MOD(vehicle, 24, func_481(vehicle, offsetGarageGlobal.at(38).value()->Int - 1), false);
			func_480(vehicle);
			if (func_479(vehicle)) {
				VEHICLE::SET_VEHICLE_STRONG(vehicle, true);
				VEHICLE::SET_VEHICLE_HAS_STRONG_AXLES(vehicle, true);
			}
			return 1;
		}
	}
	template <typename t>
	inline t getSlotTrait(int slot, int trait) {
		if constexpr (!std::is_same_v<t, const char*>) {
			return (t)rage_ysc::garage.at(slot, rage_ysc::garageSize).at(trait).value()->Any;
		}
		else {
			return (t)rage_ysc::garage.at(slot, rage_ysc::garageSize).at(trait).value()->String;
		}
	}
	template <typename t>
	inline void setSlotTrait(int slot, int trait, t value) {
		if constexpr (!std::is_same_v<t, const char*>) {
			rage_ysc::garage.at(slot, rage_ysc::garageSize).at(trait).value()->Any = value;
		}
		else {
			rage_ysc::garage.at(slot, rage_ysc::garageSize).at(trait).value()->String = value;
		}
		MISC::SET_BIT(&rage_ysc::garage.at(slot, rage_ysc::garageSize).at(static_cast<i32>(eGlobalGarageIndices::GeneralBitset)).value()->Int, 12);
	}
	inline const char* getGarageName(int id) {
		const char* label = "NULL";
		int index{ stat_get<int>(g_garageLabels[id]) };
		if (index > 0) {
			if (index > 115) {
				if (index == 116) {
					label = (rage_ysc::func_2412(rage_ysc::func_1083(PLAYER::PLAYER_ID())));
				}
				if (index == 117) {
					label = (rage_ysc::func_2412(rage_ysc::func_1081(PLAYER::PLAYER_ID())));
				}
				if (index == 118) {
					label = "MP_BHUB_CLUBG";
				}

				if (index == 119) {
					label = "MP_BHUB_GAR1";
				}
				if (index == 120) {
					label = "MP_BHUB_GAR2";
				}
				if (index == 121) {
					label = "MP_BHUB_GAR3";
				}
				if (index == 122) {
					label = "ARENA_GAR_F0";
				}
				if (index == 123) {
					label = "ARENA_GAR_F1";
				}
				if (index == 124) {
					label = "ARENA_GAR_F2";
				}
				if (index == 125) {
					label = "CASINO_GARNAME";
				}
				if (index == 126) {
					label = "ARCADE_GARNAME";
				}
				label = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(label);
			}
			else {
				label = rage_ysc::get_location_from_id(index);
			}
		}
		return label;
	}
	class garage {
	public:
		class vehicle {
		public:
			void populate(i32 i) {
				m_slot = i;
				m_model = getSlotTrait<u32>(m_slot, static_cast<i32>(eGlobalGarageIndices::Model));
				m_make = VEHICLE::GET_MAKE_NAME_FROM_VEHICLE_MODEL(m_model);
				m_name = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(m_model);
				m_makeLabel = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(m_make.c_str());
				m_nameLabel = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(m_name.c_str());
				std::stringstream str{};
				if (!m_makeLabel.empty() && m_makeLabel != "NULL") {
					str << m_makeLabel << " ";
				}
				str << m_nameLabel;
				m_label = str.str();
				m_mods.populate(m_slot);
			}
			void populateFromVehicle(i32 slot, Vehicle veh) {
				CVehicle* cVehicle{ (CVehicle*)util::classes::getEntityFromSGUID(veh) };
				CVehicleModelInfo* modelInfo{ cVehicle->get_model_info() };
				m_slot = slot + 1;
				m_model = modelInfo->m_hash;
				setSlotTrait<u32>(m_slot, static_cast<i32>(eGlobalGarageIndices::Model), m_model);
				m_make = VEHICLE::GET_MAKE_NAME_FROM_VEHICLE_MODEL(m_model);
				m_name = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(m_model);
				m_makeLabel = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(m_make.c_str());
				m_nameLabel = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(m_name.c_str());
				std::stringstream str{};
				if (!m_makeLabel.empty() && m_makeLabel != "NULL") {
					str << m_makeLabel << " ";
				}
				str << m_nameLabel;
				m_label = str.str();
				m_mods.populateFromVehicle(veh);
				m_mods.setSlotData(m_slot);
			}

			i32 m_slot{};
			u32 m_model{};
			std::string m_make{};
			std::string m_name{};
			std::string m_makeLabel{};
			std::string m_nameLabel{};
			std::string m_label{};
			struct mods {
				void setSlotData(i32 slot) {
					setSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::ModCount), numMods);
					setSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::PlateType), plateType);
					setSlotTrait<const char*>(slot, static_cast<i32>(eGlobalGarageIndices::PlateText), plateText);
					setSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::WheelType), wheelType);
					setSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::WindowTint), windowTint);
					setSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::ConvertibleRoofState), convertibleRoofState);
					setSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::AppearanceBitset), appearanceBitset);
					setSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::PaintPearlescant), pearlescant);
					setSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::PaintRim), rimPaint);
					setSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::PaintPrimary), primaryPaint);
					setSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::PaintSecondary), secondaryPaint);
					smoke.to_script(slot, static_cast<i32>(eGlobalGarageIndices::SmokeR));
					neon.to_script(slot, static_cast<i32>(eGlobalGarageIndices::NeonR));
					customColor.to_script(slot, static_cast<i32>(eGlobalGarageIndices::CustomR));
				}
				void populateFromVehicle(Vehicle veh) {
					numMods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 0);
					plateType = VEHICLE::GET_VEHICLE_PLATE_TYPE(veh);
					plateText = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
					wheelType = VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh);
					convertibleRoofState = VEHICLE::GET_CONVERTIBLE_ROOF_STATE(veh);
					if ((convertibleRoofState == 0 || convertibleRoofState == 3) || convertibleRoofState == 5) {
						isConvertibleUp = false;
					}
					else {
						isConvertibleUp = true;
					}
					leftNeonLight = VEHICLE::GET_VEHICLE_NEON_ENABLED(veh, 0);
					rightNeonLight = VEHICLE::GET_VEHICLE_NEON_ENABLED(veh, 1);
					frontNeonLight = VEHICLE::GET_VEHICLE_NEON_ENABLED(veh, 2);
					rearNeonLight = VEHICLE::GET_VEHICLE_NEON_ENABLED(veh, 3);
					if (leftNeonLight) {
						MISC::SET_BIT(&appearanceBitset, 30);
					}
					if (rightNeonLight) {
						MISC::SET_BIT(&appearanceBitset, 31);
					}
					if (frontNeonLight) {
						MISC::SET_BIT(&appearanceBitset, 28);
					}
					if (rearNeonLight) {
						MISC::SET_BIT(&appearanceBitset, 29);
					}
					VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pearlescant, &rimPaint);
					if (pearlescant < 0) {
						pearlescant = 0;
					}
					if (rimPaint < 0) {
						rimPaint = 0;
					}
					VEHICLE::GET_VEHICLE_COLOURS(veh, &primaryPaint, &secondaryPaint);
					VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &smoke.r, &smoke.g, &smoke.b);
					VEHICLE::GET_VEHICLE_NEON_COLOUR(veh, &neon.r, &neon.g, &neon.b);
					customColorIsPrimary = VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(veh);
					customColorIsSecondary = VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(veh);
					if (customColorIsPrimary) {
						MISC::SET_BIT(&appearanceBitset, 13);
					}
					if (customColorIsSecondary) {
						MISC::SET_BIT(&appearanceBitset, 12);
					}
				}
				void populate(i32 slot) {
					numMods = getSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::ModCount));
					plateType = getSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::PlateType));
					plateText = getSlotTrait<const char*>(slot, static_cast<i32>(eGlobalGarageIndices::PlateText));
					wheelType = getSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::WheelType));
					windowTint = getSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::WindowTint));
					convertibleRoofState = getSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::ConvertibleRoofState));
					if ((convertibleRoofState == 0 || convertibleRoofState == 3) || convertibleRoofState == 5) {
						isConvertibleUp = false;
					}
					else {
						isConvertibleUp = true;
					}
					appearanceBitset = getSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::AppearanceBitset));
					leftNeonLight = rage_ysc::IS_BIT_SET(appearanceBitset, 30);
					rightNeonLight = rage_ysc::IS_BIT_SET(appearanceBitset, 31);
					frontNeonLight = rage_ysc::IS_BIT_SET(appearanceBitset, 28);
					rearNeonLight = rage_ysc::IS_BIT_SET(appearanceBitset, 29);
					pearlescant = getSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::PaintPearlescant));
					if (pearlescant < 0) {
						pearlescant = 0;
					}
					rimPaint = getSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::PaintRim));
					if (rimPaint < 0) {
						rimPaint = 0;
					}
					primaryPaint = getSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::PaintPrimary));
					secondaryPaint = getSlotTrait<i32>(slot, static_cast<i32>(eGlobalGarageIndices::PaintSecondary));
					smoke.from_script(slot, static_cast<i32>(eGlobalGarageIndices::SmokeR));
					neon.from_script(slot, static_cast<i32>(eGlobalGarageIndices::NeonR));
					customColor.from_script(slot, static_cast<i32>(eGlobalGarageIndices::CustomR));
					customColorIsPrimary = rage_ysc::IS_BIT_SET(appearanceBitset, 13);
					customColorIsSecondary = rage_ysc::IS_BIT_SET(appearanceBitset, 12);
				}
				void set(Vehicle vehicle, i32 slot) {
					VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle, plateType);
					VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(vehicle, plateText);
					VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, wheelType);
					rage_ysc::apply_vehicle_mods(vehicle, slot);
					VEHICLE::SET_VEHICLE_WINDOW_TINT(vehicle, windowTint);
					if (!isConvertibleUp) {
						VEHICLE::RAISE_CONVERTIBLE_ROOF(vehicle, TRUE);
					}
					else {
						VEHICLE::LOWER_CONVERTIBLE_ROOF(vehicle, TRUE);
					}
					VEHICLE::SET_VEHICLE_NEON_ENABLED(vehicle, 0, leftNeonLight);
					VEHICLE::SET_VEHICLE_NEON_ENABLED(vehicle, 1, rightNeonLight);
					VEHICLE::SET_VEHICLE_NEON_ENABLED(vehicle, 2, frontNeonLight);
					VEHICLE::SET_VEHICLE_NEON_ENABLED(vehicle, 3, rearNeonLight);
					VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle, pearlescant, rimPaint);
					if (primaryPaint != -1 && secondaryPaint != -1) {
						VEHICLE::SET_VEHICLE_COLOURS(vehicle, primaryPaint, secondaryPaint);
					}
					VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(vehicle, smoke.r, smoke.g, smoke.b);
					VEHICLE::SET_VEHICLE_NEON_COLOUR(vehicle, neon.r, neon.g, neon.b);
					if (customColorIsPrimary) {
						VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle, customColor.r, customColor.g, customColor.b);
					}
					if (customColorIsSecondary) {
						VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle, customColor.r, customColor.g, customColor.b);
					}
				}
				struct color {
					i32 r{};
					i32 g{};
					i32 b{};
					void from_script(int slot, int trait) {
						r = getSlotTrait<i32>(slot, trait);
						g = getSlotTrait<i32>(slot, trait + 1);
						b = getSlotTrait<i32>(slot, trait + 2);
					}
					void to_script(int slot, int trait) {
						setSlotTrait<i32>(slot, trait, r);
						setSlotTrait<i32>(slot, trait + 1, g);
						setSlotTrait<i32>(slot, trait + 2, b);
					}
				};
				i32 numMods{};
				i32 plateType{};
				const char* plateText{};
				i32 wheelType{};
				i32 windowTint{};
				i32 convertibleRoofState{};
				bool isConvertibleUp{};
				i32 appearanceBitset{};
				bool leftNeonLight{};
				bool rightNeonLight{};
				bool frontNeonLight{};
				bool rearNeonLight{};
				i32 pearlescant{};
				i32 rimPaint{};
				i32 primaryPaint{};
				i32 secondaryPaint{};
				color smoke{};
				color neon{};
				color customColor{}; //Kinda weird as you need to check the bitset to see what is stored here
				bool customColorIsPrimary{};
				bool customColorIsSecondary{};
			} m_mods;
		};
		std::string m_key{};
		u32 m_hash{};
		std::string m_name{};
		std::vector<i32> m_validSlots{};
		std::vector<vehicle> m_vehicles{};
		void addVehicle(i32 slot, Vehicle veh) {
			vehicle scriptVeh{};
			scriptVeh.populateFromVehicle(slot, veh);
		}
		void populate(int i) {
			m_key = g_garageLabels[i];
			m_hash = rage::joaat(m_key);
			m_name = getGarageName(i);
			//Populate valid slots table
			i32 ivar4{ rage_ysc::func_594(i) };
			for (i32 j{}; j != rage_ysc::get_garage_vehicle_max_count(i, -1, TRUE); ++j) {
				i32 slot{};
				rage_ysc::func_210(j + ivar4, &slot, TRUE);
				if (slot >= 0 && getSlotTrait<u32>(slot, static_cast<i32>(eGlobalGarageIndices::Model)) != 0 && STREAMING::IS_MODEL_IN_CDIMAGE(getSlotTrait<u32>(slot, static_cast<i32>(eGlobalGarageIndices::Model)))) {
					m_validSlots.push_back(slot);
				}
			}
			//Populate vehicles
			if (!m_validSlots.empty()) {
				for (auto& slot : m_validSlots) {
					vehicle v{};
					v.populate(slot);
					m_vehicles.push_back(v);
				}
			}
			if (m_name == "NULL" || m_name.empty()) {
				m_name.clear();
				for (auto& veh : m_vehicles) {
					if (veh.m_label.find("Avenger") != std::string::npos) {
						m_name == "Facility";
					}
					else if (veh.m_label.find("Hauler Custom") != std::string::npos) {
						m_name == "Bunker";
					}
				}
				if (m_name == "NULL" || m_name.empty()) {
					m_name = "Garage_" + std::to_string(i);
				}
			}
		}
	};
	class garageMgr {
	public:
		void populate() {
			for (i32 i{}; i != 29; ++i) {
				garage g{};
				g.populate(i);
				m_garages.push_back(g);
			}
		}
		bool empty() {
			return m_garages.empty();
		}
		garage* get(i32 index) {
			return &m_garages.at(index);
		}
		garage* getByName(std::string name) {
			for (auto& garage : m_garages) {
				if (garage.m_name == name) {
					return &garage;
				}
			}

			return nullptr;
		}
		garage* getByHash(u32 hash) {
			for (auto& garage : m_garages) {
				if (garage.m_hash == hash) {
					return &garage;
				}
			}

			return nullptr;
		}
		std::vector<garage>garages() {
			return m_garages;
		}
	private:
		std::vector<garage> m_garages{};
	};
	inline garageMgr g_garageMgr{};
}