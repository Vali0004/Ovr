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
			extern void tinyPed(toggleCommand* command);
			extern void autoHeal(toggleCommand* command);
			extern void instantRespawn(toggleCommand* command);
			inline Vector3 g_coordsAtDeath{};
			extern void keepLastCoordinatesOnDeath(toggleCommand* command);
			extern void infiniteOxygen(toggleCommand* command);
			extern void clearTasks(actionCommand* command);
			extern void clone(actionCommand* command);
			extern void suicide(actionCommand* command);
		}
		namespace movement {
			extern void run(toggleFloatCommand* command);
			extern void swim(toggleFloatCommand* command);
			extern void stamina(toggleFloatCommand* command);
			extern void staminaRegen(toggleFloatCommand* command);
			extern void gracefulLanding(toggleCommand* command); //Handled in hooking
			extern void beastJump(toggleCommand* command); //Handled in hooking
			extern void superJump(toggleCommand* command);
			extern void ultraJump(toggleCommand* command);
			extern void superRun(toggleFloatCommand* command);
			extern void noClip(toggleFloatCommand* command);
			extern void coordSmash(toggleCommand* command);
			extern void autoTeleportToWaypoint(toggleCommand* command);
			extern void walkOnAir(toggleCommand* command);
		}
		namespace world {
			extern void walkThroughWater(toggleCommand* command);
			extern void walkOnWater(toggleCommand* command);
			extern void slowMotion(toggleCommand* command);
			extern void teleportToCoords(variadicCommand* command);
		}
		namespace police {
			extern void neverWanted(toggleCommand* command);
			extern void lockWantedLevel(toggleCommand* command);
			extern void wantedLevel(intCommand* command);
			extern void fakeWantedLevel(intCommand* command);
		}
		extern void alpha(intCommand* command);
		extern void invisibility(toggleCommand* command);
		extern void noCollision(toggleCommand* command);
		extern void noRagdoll(toggleCommand* command);
	}
	namespace weapon {
		namespace ammo {
			namespace special {
				extern void armorPiercing(toggleCommand* command);
				extern void explosive(toggleCommand* command);
				extern void fullMetalJacket(toggleCommand* command);
				extern void hollowPoint(toggleCommand* command);
				extern void incendiary(toggleCommand* command);
				extern void tracer(toggleCommand* command);
			}
			extern void infinite(toggleCommand* command);
			extern void infiniteClip(toggleCommand* command);
			extern void infiniteStickyBombs(toggleCommand* command);
			extern void infiniteFlares(toggleCommand* command);
			extern void refill(actionCommand* command);
		}
		namespace loadout {

		}
		extern void teleportGun(toggleCommand* command);
		extern void deleteGun(toggleCommand* command);
		extern void deadEye(toggleCommand* command);
	}
	namespace vehicle {
		namespace spawner {
			inline Vehicle g_lastSpawnedVehicle{};
			extern void spawnInsideVehicle(toggleCommand* command);
			extern void spawnVehicleMaxed(toggleCommand* command);
			extern void deleteLastSpawnedVehicle(toggleCommand* command);
			extern void spawnVehicle(hashCommand* command);
		}
	}
	namespace network {
		namespace friends {
			namespace selected {
				extern void copyRid(actionCommand* command);
				extern void copyName(actionCommand* command);
				extern void join(actionCommand* command);
			}
			extern void remove(stringCommand* command);
		}
		namespace session {
			namespace browser {
				namespace backend {
					class sessionBrowser {
					public:
						bool matchmake(std::optional<int> constraint = std::nullopt);
					public:
						struct sessionAttributes {
							i32 discriminator{};
							i32 playerCount{};
							i32 region{};
							i32 language{};
						};
						struct sessionData {
							rage::rlSessionInfo info{};
							sessionAttributes attributes{};
							bool valid{};
							operator bool() {
								return valid;
							}
						};
					public:
						i32 count() { return m_count; }
						sessionData* sessions() { return m_sessions; }
						bool active() { return m_active; }
					private:
						constexpr static int MaxSessions{ 1063 };
						int m_count{};
						bool m_active{};
						sessionData m_sessions[MaxSessions]{};
					};
					inline sessionBrowser g_browser{};
				}
				extern void matchmakingRegion(toggleIntCommand* command);
				extern void matchmakingLanguage(toggleIntCommand* command);
				extern void matchmakingPlayerCountMinimum(intCommand* command);
				extern void matchmakingPlayerCountMaximum(intCommand* command);
				extern void matchmakingType(toggleIntCommand* command);
			}
			namespace starter {
				extern void go(stringCommand* command);
				extern void leave(actionCommand* command);
				extern void seamlessTransition(toggleCommand* command);
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
				extern u64 getRidFromCommand(const std::string& str);
			}
			extern void ridToName(variadicCommand* command);
			extern void nameToRid(stringCommand* command);
			extern void convert(stringCommand* command);
			extern void scMessage(variadicCommand* command);
		}
		namespace tunables {
			extern void offRadar(toggleCommand* command);
		}
		extern void join(stringCommand* command);
		extern void forceHostOfScript(hashCommand* command);
		extern void forceScriptHost(actionCommand* command);
		extern void bail(actionCommand* command);
	}
	namespace protections {
		namespace kicks {
			namespace reactions {
				extern void kick(toggleCommand* command);
				extern void crash(toggleCommand* command);
			}
			extern void allKickProtections(sectionProtectionCommand* command);
		}
		namespace crashes {
			namespace reactions {
				extern void kick(toggleCommand* command);
				extern void crash(toggleCommand* command);
			}
			extern void allCrashProtections(sectionProtectionCommand* command);
		}
		namespace networkEvents {
			namespace reactions {
				extern void kick(toggleCommand* command);
				extern void crash(toggleCommand* command);
			}
			extern void allNetworkEventProtections(sectionProtectionCommand* command);
		}
		namespace scriptEvents {
			namespace reactions {
				extern void redirect(toggleCommand* command);
				extern void kick(toggleCommand* command);
				extern void crash(toggleCommand* command);
			}
			extern void allScriptEventProtections(sectionProtectionCommand* command);
		}
		namespace general {
			namespace reactions {
				extern void kick(toggleCommand* command);
				extern void crash(toggleCommand* command);
			}
			extern void allGeneralProtections(sectionProtectionCommand* command);
		}
	}
	namespace miscellaneous {
		namespace game {
			extern void mobileRadio(toggleCommand* command);
			extern void autoMp(toggleCommand* command);
			extern void exitInstantly(toggleCommand* command);
			extern void setTimeScale(floatCommand* command);
		}
		namespace world {
			extern void teleportToWaypoint(actionCommand* command);
		}
		namespace vfx {
			namespace wheel {
				inline CVFXWheel* g_vfxWeelCache{};
				extern void tireTracksColor(toggleColorCommand* command);
			}
			extern void azimuthEastIntensity(toggleFloatCommand* command);
			extern void azimuthWestIntensity(toggleFloatCommand* command);
			extern void azimuthTransitionIntensity(toggleFloatCommand* command);
			extern void azimuthTransitionPosition(toggleFloatCommand* command);
			extern void zenithIntensity(toggleFloatCommand* command);
			extern void skyPlaneParams(toggleIntCommand* command);
			extern void sunDirection(toggleIntCommand* command);
			extern void sunPosition(toggleIntCommand* command);
			extern void cloudGenerationFrequency(toggleIntCommand* command);
			extern void speedConstants(toggleIntCommand* command);
			extern void horizonLevel(toggleFloatCommand* command);
			extern void starfieldIntensity(toggleFloatCommand* command);
			extern void moonIntensity(toggleFloatCommand* command);
			extern void lunarCycle(toggleIntCommand* command);
			extern void moonDirection(toggleIntCommand* command);
			extern void moonPosition(toggleIntCommand* command);
			extern void noiseFrequency(toggleFloatCommand* command);
			extern void noiseScale(toggleFloatCommand* command);
			extern void noiseThreshold(toggleFloatCommand* command);
			extern void noiseSoftness(toggleFloatCommand* command);
			extern void noiseDensityOffset(toggleFloatCommand* command);
		}
	}
	namespace settings {
		namespace game {
			extern void unload(actionCommand* command);
			extern void exit(actionCommand* command);
		}
		namespace statistics {
			extern void drawNativesInvokedByGame(toggleCommand* command);
			extern void drawNativesInvokedByMenu(toggleCommand* command);
			extern void drawPoolCounts(toggleCommand* command);
			extern void drawCoordinates(toggleCommand* command);
			extern void drawHeading(toggleCommand* command);
			extern void drawPlayerCount(toggleCommand* command);
			extern void drawIncomingNetworkEventCount(toggleCommand* command);
			extern void drawFrameData(toggleCommand* command);
			extern void drasFramesPerSecond(toggleCommand* command);
			extern void drawSessionType(toggleCommand* command);
			extern void drawHostName(toggleCommand* command);
			extern void drawNextHostName(toggleCommand* command);
			extern void drawScriptHostName(toggleCommand* command);
			extern void drawLastScriptEventSenderName(toggleCommand* command);
			extern void drawLocalPlayerName(toggleCommand* command);
		}
		namespace ui {
			extern void scale(floatCommand* command);
		}
	}
	extern void init();
	extern void onInit();
	extern void onTick();
}