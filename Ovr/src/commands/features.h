#pragma once
#include "fiber/fiber.h"
#include "fiber/manager.h"
#include "fiber/pool.h"
#include "memory/pointers.h"
#include "rage/classes.h"
#include "rage/tables.h"
#include "rage/commands/list.h"
#include "manager/manager.h"
#include "engine/engine.h"
#include "math.h"
#include "json/json.h"
#include "util/util.h"
#include "util/clipboard.h"
#include "util/curl_wrapper.h"
#include "util/global.h"

namespace commands::features {
	inline CPed* cPed{};
	inline CPedWeaponManager* cPedWeaponManager{};
	inline CWeaponInfo* cWeaponInfo{};
	inline CAmmoInfo* cAmmoInfo{};
	inline CVehicle* cVehicle{};
	inline CPlayerInfo* cPlayerInfo{};
	namespace self {
		namespace ped {
			namespace proofs {
				extern void bullet(toggleCommand* command);
				extern void fire(toggleCommand* command);
				extern void collision(toggleCommand* command);
				extern void melee(toggleCommand* command);
				extern void god(toggleCommand* command);
				extern void explosion(toggleCommand* command);
				extern void steam(toggleCommand* command);
				extern void drown(toggleCommand* command);
				extern void water(toggleCommand* command);
			}
			namespace health {
				extern void maxAmount(actionCommand* command);
			}
			namespace armor {
				extern void maxAmount(actionCommand* command);
			}
			extern void suicide(actionCommand* command);
		}
		namespace movement {
			extern void run(toggleFloatCommand* command);
			extern void swim(toggleFloatCommand* command);
			extern void stamina(toggleFloatCommand* command);
			extern void staminaRegen(toggleFloatCommand* command);
			extern void beastJump(toggleCommand* command); //Handled in hooking
			extern void superJump(toggleCommand* command); //Handled in hooking
			extern void gracefulLanding(toggleCommand* command); //Handled in hooking
			extern void superRun(toggleFloatCommand* command);
			extern void noClip(toggleFloatCommand* command);
		}
		namespace police {
			extern void neverWanted(toggleCommand* command);
			extern void wantedLevel(intCommand* command);
		}
		extern void alpha(intCommand* command);
		extern void invisibility(toggleCommand* command);
		extern void noRagdoll(toggleCommand* command);
	}
	namespace weapon {
		namespace ammo {
			namespace special {
				inline eAmmoSpecialType o_specialAmmo{ eAmmoSpecialType::Invalid };
				inline uint32_t w_hash{};
				extern void explosive(toggleCommand* command);
			}
			extern void infinite(toggleCommand* command);
			extern void infiniteClip(toggleCommand* command);
			extern void refill(actionCommand* command);
		}
	}
	namespace vehicle {
		namespace spawner {

		}
	}
	namespace network {
		namespace friends {
			namespace selected {
				extern void copyRid(actionCommand* command);
				extern void copyName(actionCommand* command);
			}
			extern void remove(variadicCommand* command);
		}
		namespace session {
			namespace starter {
				extern void go(variadicCommand* command);
				extern void leave(actionCommand* command);
			}
			namespace players {
				namespace selected {

				}
			}
		}
		namespace socialclub {
			namespace backend {
				extern nlohmann::json jRequest(nlohmann::json body, std::string endpoint);
				extern bool isOnline(u64 rid);
				extern u64 nameToRid(std::string name);
				extern std::string ridToName(u64 rid);
				extern void getGamerTask(u64 rid, std::function<void(rage::rlSessionByGamerTaskResult&)> onSuccess);
			}
			extern void ridToName(variadicCommand* command);
			extern void nameToRid(variadicCommand* command);
			extern void convert(variadicCommand* command);
			extern void scMessage(variadicCommand* command);
		}
		namespace tunables {
			extern void offRadar(toggleCommand* command);
		}
		extern void join(variadicCommand* command);
		extern void bail(actionCommand* command);
	}
	namespace settings {
		namespace game {
			extern void autoMp(toggleCommand* command);
			extern void exitInstantly(toggleCommand* command);
			extern void unload(actionCommand* command);
			extern void exit(actionCommand* command);
		}
		namespace ui {
			extern void scale(floatCommand* command);
		}
	}
	extern void init();
	extern void onInit();
	extern void onTick();
}