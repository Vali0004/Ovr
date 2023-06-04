#pragma once
#include "fiber/fiber.h"
#include "fiber/manager.h"
#include "fiber/pool.h"
#include "memory/pointers.h"
#include "rage/classes.h"
#include "rage/tables.h"
#include "commands/list.h"
#include "manager/manager.h"
#include "manager/command_engine.h"
#include "math.h"
#include "json/json.h"
#include "util/util.h"
#include "util/clipboard.h"
#include "util/curl_wrapper.h"
#include "util/global.h"

namespace features {
	inline CPed* cPed{};
	inline CPedWeaponManager* cPedWeaponManager{};
	inline CWeaponInfo* cWeaponInfo{};
	inline CAmmoInfo* cAmmoInfo{};
	inline CVehicle* cVehicle{};
	inline CPlayerInfo* cPlayerInfo{};
	namespace self {
		namespace ped {
			namespace proofs {
				extern void bullet(toggleFeature* feature);
				extern void fire(toggleFeature* feature);
				extern void collision(toggleFeature* feature);
				extern void melee(toggleFeature* feature);
				extern void god(toggleFeature* feature);
				extern void explosion(toggleFeature* feature);
				extern void steam(toggleFeature* feature);
				extern void drown(toggleFeature* feature);
				extern void water(toggleFeature* feature);
			}
			namespace health {
				extern void maxAmount(features::actionFeature* feature);
			}
			namespace armor {
				extern void maxAmount(features::actionFeature* feature);
			}
			extern void suicide(features::actionFeature* feature);
		}
		namespace movement {
			extern void run(toggleFloatFeature* feature);
			extern void swim(toggleFloatFeature* feature);
			extern void stamina(toggleFloatFeature* feature);
			extern void staminaRegen(toggleFloatFeature* feature);
			extern void beastJump(toggleFeature* feature); //Handled in hooking
			extern void superJump(toggleFeature* feature); //Handled in hooking
			extern void gracefulLanding(toggleFeature* feature); //Handled in hooking
			extern void superRun(toggleFloatFeature* feature);
			extern void noClip(toggleFloatFeature* feature);
		}
		namespace police {
			extern void neverWanted(toggleFeature* feature);
			extern void wantedLevel(intFeature* feature);
		}
		extern void alpha(intFeature* feature);
		extern void invisibility(toggleFeature* feature);
		extern void noRagdoll(toggleFeature* feature);
	}
	namespace weapon {
		namespace ammo {
			namespace special {
				inline eAmmoSpecialType o_specialAmmo{ eAmmoSpecialType::Invalid };
				inline uint32_t w_hash{};
				extern void explosive(toggleFeature* feature);
			}
			extern void infinite(toggleFeature* feature);
			extern void infiniteClip(toggleFeature* feature);
			extern void refill(actionFeature* feature);
		}
	}
	namespace vehicle {
		namespace spawner {

		}
	}
	namespace network {
		namespace friends {
			namespace selected {
				extern void copyRid(actionFeature* feature);
				extern void copyName(actionFeature* feature);
			}
			extern void remove(variadicFeature* feature);
		}
		namespace session {
			namespace starter {
				extern void go(variadicFeature* feature);
				extern void leave(actionFeature* feature);
			}
			namespace players {
				namespace selected {

				}
			}
		}
		namespace socialclub {
			namespace backend {
				extern nlohmann::json jRequest(nlohmann::json body, std::string endpoint);
				extern bool isOnline(uint64_t rid);
				extern uint64_t nameToRid(std::string name);
				extern std::string ridToName(uint64_t rid);
			}
			extern void ridToName(variadicFeature* feature);
			extern void nameToRid(variadicFeature* feature);
		}
		namespace tunables {
			extern void offRadar(toggleFeature* feature);
		}
		extern void bail(actionFeature* feature);
	}
	namespace settings {
		namespace game {
			extern void autoMp(toggleFeature* feature);
			extern void unload(actionFeature* feature);
			extern void exit(actionFeature* feature);
		}
		namespace ui {
			extern void scale(floatFeature* feature);
		}
	}
	namespace commands {
		extern void copyText(variadicFeature* feature);
	}
	extern void onTick();
}