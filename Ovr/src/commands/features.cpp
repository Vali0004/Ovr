#include "features.h"
#include "script/script.h"
#include "hooking/hooking.h"
#include "renderer/renderer.h"
#include "script/elements.h"
#include "commands/gui/gui.h"

namespace commands::features {
	namespace self {
		namespace ped {
			namespace proofs {
				void setAllProofs(actionCommand* command) {
					"bulletProof"_TC->get(0).toggle ^= true;
					"fireProof"_TC->get(0).toggle ^= true;
					"collisionProof"_TC->get(0).toggle ^= true;
					"meleeProof"_TC->get(0).toggle ^= true;
					"godProof"_TC->get(0).toggle ^= true;
					"explosionProof"_TC->get(0).toggle ^= true;
					"steamProof"_TC->get(0).toggle ^= true;
					"drownProof"_TC->get(0).toggle ^= true;
					"waterProof"_TC->get(0).toggle ^= true;
				}
				void bullet(toggleCommand* command) {
					cPed->set_damage_bit(eEntityProofs::Bullet, command->get(0).toggle);
				}
				void fire(toggleCommand* command) {
					cPed->set_damage_bit(eEntityProofs::Fire, command->get(0).toggle);
				}
				void collision(toggleCommand* command) {
					cPed->set_damage_bit(eEntityProofs::Collision, command->get(0).toggle);
				}
				void melee(toggleCommand* command) {
					cPed->set_damage_bit(eEntityProofs::Melee, command->get(0).toggle);
				}
				void god(toggleCommand* command) {
					cPed->set_damage_bit(eEntityProofs::God, command->get(0).toggle);
				}
				void explosion(toggleCommand* command) {
					cPed->set_damage_bit(eEntityProofs::Explosion, command->get(0).toggle);
				}
				void steam(toggleCommand* command) {
					cPed->set_damage_bit(eEntityProofs::Steam, command->get(0).toggle);
				}
				void drown(toggleCommand* command) {
					cPed->set_damage_bit(eEntityProofs::Drown, command->get(0).toggle);
				}
				void water(toggleCommand* command) {
					cPed->set_damage_bit(eEntityProofs::Water, command->get(0).toggle);
				}
			}
			namespace health {
				void maxAmount(actionCommand* command) {
					cPed->m_health = cPed->m_maxhealth;
				}
			}
			namespace armor {
				void maxAmount(actionCommand* command) {
					cPed->m_armour = cPlayerInfo->m_maxarmour;
				}
			}
			void tinyPed(toggleCommand* command) {
				Ped ped{ PLAYER::PLAYER_PED_ID() };
				PED::SET_PED_CONFIG_FLAG(ped, 223, command->get(0).toggle);
			}
			void autoHeal(toggleCommand* command) {
				if (command->get(0).toggle) {
					"maxHealth"_AC->run();
					"maxArmor"_AC->run();
				}
			}
			void instantRespawn(toggleCommand* command) {
				if (command->get(0).toggle) {
					Ped ped{ PLAYER::PLAYER_PED_ID() };
					if (PED::IS_PED_DEAD_OR_DYING(ped, FALSE)) {
						Vector3 pos{ ENTITY::GET_ENTITY_COORDS(ped, TRUE) };
						if (util::network::g_manager.online()) {
							NETWORK::NETWORK_RESURRECT_LOCAL_PLAYER(pos, 0.f, FALSE, FALSE, FALSE, -1, -1);
						}
						else {
							PED::RESURRECT_PED(ped);
							ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, pos, FALSE, FALSE, FALSE);
						}
						MISC::FORCE_GAME_STATE_PLAYING();
						"clearTasks"_AC->run();
						MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("respawn_controller");
					}
				}
			}
			void keepLastCoordinatesOnDeath(toggleCommand* command) {
				Ped ped{ PLAYER::PLAYER_PED_ID() };
				if (PED::IS_PED_DEAD_OR_DYING(ped, FALSE)) {
					Vector3 pos{ ENTITY::GET_ENTITY_COORDS(ped, TRUE) };
					g_coordsAtDeath = pos;
				}
			}
			void infiniteOxygen(toggleCommand* command) {
				if (command->get(0).toggle) {
					cPed->m_ped_intelligence->m_oxygen_time = 0.f;
				}
			}
			void clearTasks(actionCommand* command) {
				Ped ped{ PLAYER::PLAYER_PED_ID() };
				TASK::CLEAR_PED_TASKS_IMMEDIATELY(ped);
			}
			void clone(actionCommand* command) {
				util::classes::getPedFactory()->ClonePed(cPed, true, true, false);
			}
			void suicide(actionCommand* command) {
				cPed->m_health = 0.f;
			}
		}
		namespace movement {
			void run(toggleFloatCommand* command) {
				cPlayerInfo->m_run_speed = command->get(0).toggle ? command->get(1).floating_point : 1.f;
			}
			void swim(toggleFloatCommand* command) {
				cPlayerInfo->m_swim_speed = command->get(0).toggle ? command->get(1).floating_point : 1.f;
			}
			void stamina(toggleFloatCommand* command) {
				cPlayerInfo->m_stamina = command->get(0).toggle ? command->get(1).floating_point : 11.f;
			}
			void staminaRegen(toggleFloatCommand* command) {
				cPlayerInfo->m_stamina_regen = command->get(0).toggle ? command->get(1).floating_point : 1.f;
			}
			void gracefulLanding(toggleCommand* command) {

			}
			void beastJump(toggleCommand* command) {

			}
			void superJump(toggleCommand* command) {
				Player player{ PLAYER::PLAYER_ID() };
				if (command->get(0).toggle) {
					MISC::SET_SUPER_JUMP_THIS_FRAME(player);
				}
			}
			void ultraJump(toggleCommand* command) {
				if (command->get(0).toggle) {
					Ped ped{ PLAYER::PLAYER_PED_ID() };
					static bool wasJumping{};
					static int isJumpingTimer{};
					bool isJumping = PED::IS_PED_JUMPING(ped);
					if (isJumping && wasJumping) {
						wasJumping = true;
						if (isJumpingTimer > 0) {
							isJumpingTimer--;
							PED::SET_PED_CAN_RAGDOLL(ped, FALSE);
							WEAPON::REMOVE_WEAPON_FROM_PED(ped, "GADGET_PARACHUTE"_joaat);
							ENTITY::APPLY_FORCE_TO_ENTITY(ped, 1, { 0.f, 2.5f, 20.f }, {}, 0, TRUE, TRUE, TRUE, FALSE, TRUE);
						}
					}
					else if (isJumping && !wasJumping) {
						wasJumping = true;
						isJumpingTimer = 10;
					}
					else if (!isJumping) {
						wasJumping = false;
					}
				}
			}
			void superRun(toggleFloatCommand* command) {
				if (command->get(0).toggle) {
					if (PAD::IS_DISABLED_CONTROL_PRESSED(2, 32) && PAD::IS_DISABLED_CONTROL_PRESSED(2, 21)) {
						Player player{ PLAYER::PLAYER_ID() };
						Ped ped{ PLAYER::PLAYER_PED_ID() };
						ENTITY::APPLY_FORCE_TO_ENTITY(ped, 1, { 0.f, command->get(1).floating_point, 0.f }, {}, 0, TRUE, TRUE, TRUE, FALSE, FALSE);
						PLAYER::SET_PLAYER_SPRINT(player, TRUE);
						PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 5.f);
					}
				}
			}
			void noClip(toggleFloatCommand* command) {
				Entity entity{ PLAYER::PLAYER_PED_ID() };
				rage::CDynamicEntity* ent{ cPed };
				if (command->get(0).toggle && g_running) {
					if (PED::IS_PED_IN_ANY_VEHICLE(entity, FALSE)) {
						entity = PED::GET_VEHICLE_PED_IS_IN(entity, FALSE);
						ent = cVehicle;
					}
					ent->getPhBoundComposite()->m_bounds[0]->m_bounding_box_max_xyz_margin_w.w = -2.f;
					Vector3 playerPosition{ ent->get_position().serialize() };
					Vector3 cameraRotation{ math::rotationToDirection(CAM::GET_GAMEPLAY_CAM_ROT(0)) };
					if (PED::IS_PED_IN_ANY_VEHICLE(entity, FALSE)) {
						VEHICLE::SET_VEHICLE_FIXED(entity);
						Vector3 rotation{ CAM::GET_GAMEPLAY_CAM_ROT(0.1) };
						ENTITY::SET_ENTITY_ROTATION(entity, rotation.x, rotation.y, rotation.z, 0.1, 0.1);
					}
					Vector3 coords{ playerPosition + cameraRotation * command->get(1).floating_point };
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, playerPosition, TRUE, TRUE, FALSE);
					if (PAD::IS_DISABLED_CONTROL_PRESSED(0, eControl::ControlSprint) && !gui::g_box.m_draw) {
						coords = { playerPosition + cameraRotation * command->get(1).floating_point * 3.f };
					}
					if (PAD::IS_DISABLED_CONTROL_PRESSED(0, eControl::ControlMoveUpOnly) && !gui::g_box.m_draw) {
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, coords, TRUE, TRUE, FALSE);
					}
				}
				else {
					ent->getPhBoundComposite()->m_bounds[0]->m_bounding_box_max_xyz_margin_w.w = 0.25f;
				}
			}
			void coordSmash(toggleCommand* command) {
				static rage::vector3 pos{};
				if (command->get(0).toggle) {
					if (pos.x == 0.f || pos.y == 0.f)
						pos = cPed->get_position();
					cPed->set_position({ pos.x, pos.y, 120.f });
				}
				else {
					pos = {};
				}
			}
			void autoTeleportToWaypoint(toggleCommand* command) {
				if (command->get(0).toggle) {
					"teleportToWaypoint"_AC->run();
				}
			}
			void walkOnAir(toggleCommand* command) {
				if (command->get(0).toggle) {
					Vector3 coords{ cPed->get_position().serialize() };
					u32 hash{ "p_oil_slick_01"_joaat };
					static Object handle{};
					if (!ENTITY::DOES_ENTITY_EXIST(handle)) {
						if (!STREAMING::HAS_MODEL_LOADED(hash)) {
							STREAMING::REQUEST_MODEL(hash);
						}
						else {
							handle = OBJECT::CREATE_OBJECT(hash, coords, TRUE, TRUE, FALSE);
							ENTITY::FREEZE_ENTITY_POSITION(handle, TRUE);
							ENTITY::SET_ENTITY_VISIBLE(handle, FALSE, FALSE);
						}
					}
					else {
						if (PAD::IS_DISABLED_CONTROL_PRESSED(0, eControl::ControlSprint)) {
							coords.z -= 1.6f;
						}
						if (PAD::IS_DISABLED_CONTROL_PRESSED(0, eControl::ControlJump)) {
							coords.z -= 0.4f;
						}
						else {
							coords.z -= 1.0225f;
						}
						ENTITY::SET_ENTITY_COORDS(handle, coords, TRUE, FALSE, FALSE, FALSE);
					}
				}
			}
		}
		namespace world {
			void walkThroughWater(toggleCommand* command) {
				for (i8 i{}; command->get(0).toggle && i != 5; i++) {
					Vector2 coords{ cPed->get_position().serialize_v2() };
					if (i < 2) {
						coords.x += (i % 2 == 0) ? -4 : 4;
					}
					else if (i < 4) {
						coords.y += (i % 2 == 0) ? -4 : 4;
					}
					WATER::MODIFY_WATER(coords, -500000.f, 0.2f);
				}
			}
			void walkOnWater(toggleCommand* command) {
				if (command->get(0).toggle) {
					Vector3 coords{ cPed->get_position().serialize() };
					static float height{};
					static Object handle{};
					if (handle) {
						WATER::GET_WATER_HEIGHT(coords, &height);
						ENTITY::SET_ENTITY_VISIBLE(handle, FALSE, FALSE);
						ENTITY::SET_ENTITY_COORDS(handle, { coords.x, coords.y, height - 1.9f }, TRUE, FALSE, FALSE, TRUE);
						ENTITY::SET_ENTITY_ROTATION(handle, 180.f, 90.f, 180.f, 2, FALSE);
						ENTITY::FREEZE_ENTITY_POSITION(handle, TRUE);
					}
					else {
						util::natives::requestModel("prop_ld_ferris_wheel"_joaat);
						handle = OBJECT::CREATE_OBJECT("prop_ld_ferris_wheel"_joaat, coords, TRUE, TRUE, FALSE);
						ENTITY::FREEZE_ENTITY_POSITION(handle, TRUE);
						ENTITY::SET_ENTITY_COLLISION(handle, TRUE, FALSE);
					}
				}
			}
			void slowMotion(toggleCommand* command) {
				"setTimeScale"_FC->get(0).floating_point = command->get(0).toggle ? 0.5f : 1.f;
			}
			void teleportToCoords(variadicCommand* command) {
				Vector3 pos{ command->get(0).floating_point, command->get(1).floating_point, command->get(2).floating_point };
				Entity entity{ PLAYER::PLAYER_PED_ID() };
				if (PED::IS_PED_IN_ANY_VEHICLE(entity, FALSE)) {
					entity = PED::GET_VEHICLE_PED_IS_IN(entity, FALSE);
				}
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, pos, TRUE, TRUE, FALSE);
				float groundZCoord{};
				MISC::GET_GROUND_Z_FOR_3D_COORD(pos, &groundZCoord, FALSE, FALSE);
				pos.z = groundZCoord;
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, pos, TRUE, TRUE, FALSE);
			}
		}
		namespace police {
			void neverWanted(toggleCommand* command) {
				if (command->get(0).toggle) {
					cPlayerInfo->m_is_wanted = false;
					cPlayerInfo->m_wanted_level_display = cPlayerInfo->m_wanted_level = 0;
				}
			}
			void lockWantedLevel(toggleCommand* command) {
				if (command->get(0).toggle) {
					"wantedLevel"_IC->get(0).i32 = cPlayerInfo->m_wanted_level;
					"wantedLevel"_IC->run();
					PLAYER::SET_MAX_WANTED_LEVEL(cPlayerInfo->m_wanted_level);
				}
				else {
					PLAYER::SET_MAX_WANTED_LEVEL(5);
				}
			}
			void wantedLevel(intCommand* command) {
				cPlayerInfo->m_wanted_level = command->get(0).i32;
				cPlayerInfo->m_is_wanted = cPlayerInfo->m_wanted_level > 0;
			}
			void fakeWantedLevel(intCommand* command) {
				cPlayerInfo->m_wanted_level_display = command->get(0).i32;
			}
		}
		void alpha(intCommand* command) {
			Ped ped{ PLAYER::PLAYER_PED_ID() };
			ENTITY::SET_ENTITY_ALPHA(ped, command->get(0).toggle, FALSE);
		}
		void invisibility(toggleCommand* command) {
			Ped ped{ PLAYER::PLAYER_PED_ID() };
			ENTITY::SET_ENTITY_VISIBLE(ped, !command->get(0).toggle, FALSE);
			AUDIO::SET_PED_FOOTSTEPS_EVENTS_ENABLED(ped, !command->get(0).toggle);
			AUDIO::SET_PED_CLOTH_EVENTS_ENABLED(ped, !command->get(0).toggle);
			PED::SET_FORCE_FOOTSTEP_UPDATE(ped, command->get(0).toggle);
		}
		void noCollision(toggleCommand* command) {
			cPed->getPhBoundComposite()->m_bounds[0]->m_bounding_box_max_xyz_margin_w.w = command->get(0).toggle ? -1.f : 0.25f;
		}
		void noRagdoll(toggleCommand* command) {
			Ped ped{ PLAYER::PLAYER_PED_ID() };
			PED::SET_PED_CAN_RAGDOLL(ped, !command->get(0).toggle);
			if (command->get(0).toggle) {
				PED::SET_RAGDOLL_BLOCKING_FLAGS(ped, 2);
				cPed->m_ped_type &= ~(uint32_t)ePedType::Ragdogable;
			}
			else {
				PED::CLEAR_RAGDOLL_BLOCKING_FLAGS(ped, 2);
				cPed->m_ped_type |= (uint32_t)ePedType::Ragdogable;
			}
		}
	}
	namespace weapon {
		namespace ammo {
			namespace special {
				void explosive(toggleCommand* command) {
					if (cAmmoInfo) {
						if (command->get(0).toggle) {
							cAmmoInfo->m_ammo_special_type |= (uint32_t)eAmmoSpecialType::Explosive;
						}
						else {
							cAmmoInfo->m_ammo_special_type &= ~(uint32_t)eAmmoSpecialType::Explosive;
						}
					}
				}
			}
			void infinite(toggleCommand* command) {
				cPed->m_inventory->m_infinite_ammo = command->get(0).toggle;
			}
			void infiniteClip(toggleCommand* command) {
				cPed->m_inventory->m_infinite_clip = command->get(0).toggle;
			}
			void infiniteStickyBombs(toggleCommand* command) {
				if (command->get(0).toggle)
					cPed->m_fired_sticky_bombs = 0;
			}
			void infiniteFlares(toggleCommand* command) {
				if (command->get(0).toggle)
					cPed->m_fired_flares = 0;
			}
			void refill(actionCommand* command) {
				Ped ped{ PLAYER::PLAYER_PED_ID() };
				WEAPON::REFILL_AMMO_INSTANTLY(ped);
			}
		}
		namespace loadout {

		}
		void teleportGun(toggleCommand* command) {
			if (command->get(0).toggle) {
				Ped ped{ PLAYER::PLAYER_PED_ID() };
				if (util::raycast ray{}; ray.check(5000.f) && PED::IS_PED_SHOOTING(ped)) {
					Vector3 coords{ ray.coords() };
					g_engine.primitiveExecute("teleportToCoords {} {} {}", coords.x, coords.y, coords.z);
				}
			}
		}
		void deleteGun(toggleCommand* command) {
			if (command->get(0).toggle) {
				Ped ped{ PLAYER::PLAYER_PED_ID() };
				if (util::raycast ray{}; ray.check(5000.f) && PED::IS_PED_SHOOTING(ped)) {
					Entity entity{ ray.entity() };
					Vector3 coords{ ray.coords() };
					if (ENTITY::IS_ENTITY_A_PED(entity) && PED::IS_PED_A_PLAYER(entity)) {
						LOG_DIRECT(White, "Weapons", "You can't delete players!");
					}
					else {
						float distance{ math::distanceBetweenVecs(cPed->get_position().serialize(), coords) };
						if (distance >= 5000.f) {
							LOG_DIRECT(White, "Weapons", "The entity you're trying to delete is too far!");
						}
						else {
							if (util::natives::forcefullyTakeControl(entity)) {
								ENTITY::SET_ENTITY_AS_MISSION_ENTITY(entity, TRUE, TRUE);
								ENTITY::DELETE_ENTITY(&entity);
							}
							else {
								LOG_DIRECT(White, "Weapons", "Failed to take control of entity.");
							}
						}
					}
				}
			}
		}
		void deadEye(toggleCommand* command) {
			if (command->get(0).toggle) {
				Ped ped{ PLAYER::PLAYER_PED_ID() };
				if (cPed->on_foot()) {
					if (PAD::IS_CONTROL_PRESSED(0, ControlAim)) {
						"setTimeScale"_FC->get(0).floating_point = 0.2f;
						GRAPHICS::ANIMPOSTFX_PLAY("HeistLocate", 0, FALSE);
					}
					else if (PAD::IS_CONTROL_RELEASED(0, ControlAim) && GRAPHICS::ANIMPOSTFX_IS_RUNNING("HeistLocate")) {
						"setTimeScale"_FC->get(0).floating_point = 1.f;
						GRAPHICS::ANIMPOSTFX_STOP("HeistLocate");
					}
				}
			}
		}
	}
	namespace vehicle {
		namespace spawner {
			void spawnInsideVehicle(toggleCommand* command) {

			}
			void spawnVehicleMaxed(toggleCommand* command) {

			}
			void deleteLastSpawnedVehicle(toggleCommand* command) {

			}
			void spawnVehicle(hashCommand* command) {
				g_pool.add([command] {
					u32 hash{ command->get_hash() };
					if (!STREAMING::IS_MODEL_VALID(hash)) {
						LOG(Commands, "{} is not a valid hash!", command->get_key());
					}
					else {
						Player player{ PLAYER::PLAYER_ID() };
						Ped ped{ PLAYER::GET_PLAYER_PED(player) };
						while (!STREAMING::HAS_MODEL_LOADED(hash)) {
							STREAMING::REQUEST_MODEL(hash);
							fiber::current()->sleep(100ms);
						}
						if ("deleteLastSpawnedVehicle"_TC->get(0).toggle) {
							if (g_lastSpawnedVehicle)
								VEHICLE::DELETE_VEHICLE(&g_lastSpawnedVehicle);
						}
						Vector3 pos{ ENTITY::GET_ENTITY_COORDS(ped, TRUE) };
						float heading{ ENTITY::GET_ENTITY_HEADING(ped) };
						Vehicle vehicle{ VEHICLE::CREATE_VEHICLE(hash, pos, heading, TRUE, TRUE, FALSE) };
						g_lastSpawnedVehicle = vehicle;
						if (NETWORK::NETWORK_IS_SESSION_ACTIVE()) {
							DECORATOR::DECOR_SET_INT(vehicle, "MPBitset", 0);
							DECORATOR::DECOR_SET_INT(vehicle, "RandomID", cPed->m_net_object->m_object_id);
							i32 networkId{ NETWORK::VEH_TO_NET(vehicle) };
							if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(vehicle)) {
								NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
							}
							VEHICLE::SET_VEHICLE_IS_STOLEN(vehicle, FALSE);
						}
						if ("spawnInsideVehicle"_TC->get(0).toggle) {
							PED::SET_PED_INTO_VEHICLE(ped, vehicle, -1);
						}
						if ("spawnVehicleMaxed"_TC->get(0).toggle) {
							if (VEHICLE::IS_THIS_MODEL_A_CAR(hash) || VEHICLE::IS_THIS_MODEL_A_BIKE(hash)) {
								VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
								VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(vehicle, FALSE);
								VEHICLE::SET_VEHICLE_HAS_STRONG_AXLES(vehicle, TRUE);
								i32 numberOfEngineMods{ VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, VehicleModEngine) };
								i32 numberOfBrakeMods{ VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, VehicleModBrakes) };
								i32 numberOfTransmissionMods{ VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, VehicleModTransmission) };
								i32 numberOfSuspensionMods{ VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, VehicleModSuspension) };
								i32 numberOfArmourMods{ VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, VehicleModArmour) };
								VEHICLE::SET_VEHICLE_MOD(vehicle, VehicleModEngine, numberOfEngineMods - 1, FALSE);
								VEHICLE::SET_VEHICLE_MOD(vehicle, VehicleModBrakes, numberOfBrakeMods - 1, FALSE);
								VEHICLE::SET_VEHICLE_MOD(vehicle, VehicleModTransmission, numberOfTransmissionMods - 1, FALSE);
								VEHICLE::SET_VEHICLE_MOD(vehicle, VehicleModSuspension, numberOfSuspensionMods - 1, FALSE);
								VEHICLE::SET_VEHICLE_MOD(vehicle, VehicleModArmour, numberOfArmourMods - 1, FALSE);
								VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle, NumberPlateTypeNorthYankton);
								VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, VehicleToggleModTurbo, TRUE);
								VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, VehicleToggleModXenonHeadlights, TRUE);
							}
						}
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
					}
				});
			}
		}
	}
	namespace network {
		namespace friends {
			namespace selected {
				void copyRid(actionCommand* command) {
					static FriendInfo& info{ util::network::friends::getFriends()[g_selectedFriend] };
					g_engine.primitiveExecute("copyText {}", info.m_rockstar_id);
				}
				void copyName(actionCommand* command) {
					static FriendInfo& info{ util::network::friends::getFriends()[g_selectedFriend] };
					g_engine.primitiveExecute("copyText {}", info.m_name);
				}
				void join(actionCommand* command) {
					static FriendInfo& info{ util::network::friends::getFriends()[g_selectedFriend] };
					g_engine.primitiveExecute("join {}", info.m_name);
				}
			}
			void remove(stringCommand* command) {
				util::clipboard clipboard{};
				clipboard.get();
				std::string curText{ clipboard.str() };
				g_engine.primitiveExecute("nameToRid {}", command->get(0).string);
				clipboard.get();
				std::string rid{ clipboard.str() };
				clipboard.str() = curText;
				clipboard.set();
				nlohmann::json body = { { "RockstarId", rid } };
				auto res = socialclub::backend::jRequest(body, "https://scapi.rockstargames.com/friends/remove");
				LOG_DEBUG("Body: {}", body.dump(4));
			}
		}		
		namespace session {
			namespace browser {
				namespace backend {
					bool sessionBrowser::matchmake(std::optional<int> constraint) {
						for (auto& session : m_sessions) {
							session.valid = true;
						}
						NetworkGameFilterMatchmakingComponent component{};
						strcpy(component.m_filter_name, "Group");
						component.m_game_mode = 0;
						component.m_num_parameters = 0;
						rage::rlTaskStatus status{};
						static rage::rlSessionInfo sessions[MaxSessions]{};
						m_active = true;
						if (CALL(findGameMatch, 0, 1, &component, MaxSessions, sessions, &m_count, &status)) {
							while (status.m_state == 1) {
								fiber::current()->sleep();
							}
							if (status.m_state == 3) {
								for (i32 i{}; i != m_count; ++i) {
									sessionData s{ m_sessions[i] };
									i32 d{ s.attributes.discriminator };
									i32 pc{ s.attributes.playerCount };
									i32 r{ s.attributes.region };
									i32 l{ s.attributes.language };
									if (constraint && pc >= 30) {
										s.valid = false;
									}
									if ("matchmakingLanguage"_TIC ->get(0).toggle && l != "matchmakingLanguage"_TIC ->get(1).i32) {
										s.valid = false;
									}
									if (pc < "matchmakingPlayerCountMinimum"_IC->get(0).i32 || pc > "matchmakingPlayerCountMaximum"_IC->get(0).i32) {
										s.valid = false;
									}
									if ("matchmakingType"_TIC ->get(0).toggle && ((d & (1 << 14)) == (1 << 14)) != (bool)"matchmakingType"_TIC ->get(1).i32) {
										s.valid = false;
									}
								}
								m_active = false;
								return true;
							}
						}
						m_active = false;
						return false;
					}
				}
				void matchmakingRegion(toggleIntCommand* command) {

				}
				void matchmakingLanguage(toggleIntCommand* command) {

				}
				void matchmakingPlayerCountMinimum(intCommand* command) {

				}
				void matchmakingPlayerCountMaximum(intCommand* command) {

				}
				void matchmakingType(toggleIntCommand* command) {

				}
			}
			namespace players {
				namespace selected {

				}
			}
			namespace starter {
				void go(stringCommand* command) {
					static tables::eSessionType type{};
					std::string cid{ command->get(0).string };
					if (isNumber(cid)) {
						type = (tables::eSessionType)stoi(cid);
					}
					else {
						bool found{};
						for (int i{}; i != COUNT(tables::g_sessions); ++i) {
							auto& session{ tables::g_sessions[i] };
							if (session.name == cid) {
								type = session.type;
								found = true;
								break;
							}
							if (std::string name{ session.name }; name.find(cid) != std::string::npos) {
								type = session.type;
								found = true;
								break;
							}
							if (std::string id{ session.id }; id.find(cid) != std::string::npos) {
								type = session.type;
								found = true;
								break;
							}
						}
						if (!found) {
							LOG(Commands, "Command 'go' accepts a string but there is no type '{}'", cid);
							return;
						}
					}
					g_pool.add([] {
						global(2695915).value()->Int = type == tables::eSessionType::SCTV;
						if (type == tables::eSessionType::Leave) {
							global(1574589).at(2).value()->Int = -1;
						}
						else {
							global(1575020).value()->Int = (int)type;
						}
						global(1574589).value()->Int = true;
						fiber::current()->sleep(200ms);
						global(1574589).value()->Int = false;
					});
				}
				void leave(actionCommand* command) {
					g_engine.primitiveExecute("go -1");
					g_engine.primitiveExecute("bail");
				}
				void seamlessTransition(toggleCommand* command) {
					if (command->get(0).toggle) {
						Ped ped{ PLAYER::PLAYER_PED_ID() };
						//Forcefully draw minimap and HUD
						HUD::DISPLAY_HUD(TRUE);
						HUD::DISPLAY_RADAR(TRUE);
						HUD::DISPLAY_HUD_WHEN_NOT_IN_STATE_OF_PLAY_THIS_FRAME();
						HUD::DISPLAY_HUD_WHEN_PAUSED_THIS_FRAME();
						HUD::CLEAR_HELP(TRUE);
						//Clear tooltips when loading
						HUD::CLEAR_ALL_HELP_MESSAGES();
						//Yeet the busyspinner
						HUD::BUSYSPINNER_OFF();
						HUD::DISABLE_PAUSEMENU_SPINNER(TRUE);
						//Yeet thefeed (i.e., the loading screen tips/big fucking rectangle)
						HUD::THEFEED_HIDE();
						HUD::THEFEED_HIDE_THIS_FRAME();
						HUD::THEFEED_FLUSH_QUEUE();
						HUD::THEFEED_PAUSE();
						//Set as mission entity to avoid the script doing cleanup
						if (PED::IS_PED_IN_ANY_VEHICLE(ped, FALSE)) {
							Vehicle veh{ PED::GET_VEHICLE_PED_IS_IN(ped, FALSE) };
							ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, TRUE, TRUE);
						}
						//Does the same as above but to the last vehicle
						Vehicle veh{ PED::GET_VEHICLE_PED_IS_IN(ped, TRUE) };
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, TRUE, TRUE);
						const eTransitionState state{ static_cast<eTransitionState>(global(1574993).at(3).value()->Int) };
						if (STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS()) {
							if (state <= eTransitionState::FreemodeFinalSetupPlayer) {
								PLAYER::SET_PLAYER_CONTROL(ped, TRUE, 0);
								STREAMING::STOP_PLAYER_SWITCH();
								GRAPHICS::ANIMPOSTFX_STOP_ALL();
							}
						}
					}
				}
			}
		}
		namespace socialclub {
			namespace backend {
				size_t writeCallback(char* ptr, size_t size, size_t nmemb, std::string* data) {
					size_t totalSize = size * nmemb;
					data->append(ptr, totalSize);
					return totalSize;
				}
				nlohmann::json jRequest(nlohmann::json body, std::string endpoint) {
					std::string ticket = util::network::socialclub::getTicket();
					CURL* curl = curl_easy_init();
					if (curl) {
						std::string response;
						std::string jsonBody = body.dump();
						struct curl_slist* headers = nullptr;
						headers = curl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
						headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");
						std::string authorizationHeader = "Authorization: SCAUTH val=\"" + ticket + "\"";
						headers = curl_slist_append(headers, authorizationHeader.c_str());
						curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
						curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody.c_str());
						curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonBody.length());
						curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
						curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
						curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
						CURLcode result = curl_easy_perform(curl);
						curl_slist_free_all(headers);
						curl_easy_cleanup(curl);
						if (result == CURLE_OK) {
							return nlohmann::json::parse(response);
						}
					}
					return {};
				}
				bool isOnline(u64 rid) {
					if (nlohmann::json json{ jRequest({ { "RockstarId", std::to_string(rid) } }, "https://scui.rockstargames.com/api/friend/getprofile") }; !json.is_null()) {
						for (auto& acc : json["Accounts"]) {
							if (auto& r_acc = acc["RockstarAccount"]; !r_acc.is_null()) {
								if (rid == r_acc["RockstarId"].get<u64>()) {
									return r_acc["IsOnline"].get<bool>();
								}
							}
						}
					}
					return false;
				}
				u64 nameToRid(std::string name) {
					std::string endpoint = "https://scui.rockstargames.com/api/friend/accountsearch";
					nlohmann::json body = { { "searchNickname", name } };
					nlohmann::json json = jRequest(body, endpoint);
					if (!json.is_null()) {
						if (name.size() <= 20) {
							int numAccs = json["Total"].get<int>();
							if (numAccs > 0) {
								return json["Accounts"][0]["RockstarId"].get<u64>();
							}
							else {
								LOG(Info, "{} wasn't found. Please ensure there are no spelling mistakes.", name);
							}
						}
						else {
							LOG(Info, "The character count cannot exceed 20. Please shorten the value.");
						}
					}
					return 0;
				}
				std::string ridToName(u64 rid) {
					if (nlohmann::json json{ jRequest({ { "RockstarId", std::to_string(rid) } }, "https://scui.rockstargames.com/api/friend/getprofile") }; !json.is_null()) {
						for (auto& acc : json["Accounts"]) {
							if (auto& r_acc = acc["RockstarAccount"]; !r_acc.is_null()) {
								if (rid == r_acc["RockstarId"].get<u64>()) {
									return r_acc["Name"].get<std::string>();
								}
							}
						}
					}
					return {};
				}
				u64 getRidFromCommand(const std::string& str) {
					std::string strRid{ str };
					if (strRid.empty()) {
						LOG(Commands, "Please provide a name or RID");
						return 0;
					}
					if (!isNumber(strRid)) {
						u64 rid{ socialclub::backend::nameToRid(strRid) };
						if (!rid) {
							LOG(Commands, "Failed to get {}'s RID", strRid);
							return 0;
						}
						strRid = std::to_string(rid);
					}
					u64 rid{ stoull(strRid) };
					return rid;
				}
			}
			void ridToName(variadicCommand* command) {
				g_engine.primitiveExecute("copyText {}", backend::ridToName(command->get(0).u64));
			}
			void nameToRid(stringCommand* command) {
				util::async([command] {
					u64 rid{ backend::getRidFromCommand(command->get_string()) };
					if (rid)
						g_engine.primitiveExecute("copyText {}", rid);
				});
			}
			void convert(stringCommand* command) {
				std::string str{ command->get_string() };
				if (str.empty()) {
					LOG(Info, "Please provide an name or RID");
					return;
				}
				if (!isNumber(str)) {
					if (u64 rid{ backend::getRidFromCommand(command->get_string()) }) {
						str = std::to_string(rid);
					}
				}
				else {
					u64 rid{ stoull(str) };
					if (!rid) {
						LOG(Info, "The RID cannot be 0!", str);
						return;
					}
					str = backend::ridToName(rid);
				}
				g_engine.primitiveExecute("copyText {}", str);
			}
			void scMessage(variadicCommand* command) {
				std::string strRid{ std::to_string(backend::getRidFromCommand(command->get(0).string)) };
				std::string msg{ command->get(1).string };
				if (msg.empty()) {
					LOG(Info, "Please provide an messages");
					return;
				}
				backend::jRequest({ { "env", "prod" }, { "title", "gta5" }, { "version", 11 }, { "recipientRockstarId", strRid }, { "messageText", msg } }, "https://scui.rockstargames.com/api/messaging/sendmessage");
			}
		}
		namespace tunables {
			void offRadar(toggleCommand* command) {
				global(2657704).at(PLAYER::PLAYER_ID(), 463).at(210).value()->Int = command->get(0).toggle;
				global(2672524).at(57).value()->Int = NETWORK::GET_NETWORK_TIME() + (command->get(0).toggle ? 0xB8E10 : NULL);
			}
		}
		__declspec(align(16)) class CFriend {
		public:
			uint8_t m_state;
			uint64_t m_rid;
		};
		__declspec(align(16)) class CFriendMenu {
		public:
			virtual ~CFriendMenu() = default;
			CFriend m_friends[20];
		};
		void join(stringCommand* command) {
			u64 rid{ socialclub::backend::getRidFromCommand(command->get_string()) };
			g_pool.add([rid] {
				if (HUD::GET_CURRENT_FRONTEND_MENU_VERSION() != 0xFFFFFFFF) {
					HUD::ACTIVATE_FRONTEND_MENU("FE_MENU_VERSION_SP_PAUSE"_joaat, false, 2);
					fiber::current()->sleep(200ms);
				}
				HUD::ACTIVATE_FRONTEND_MENU("FE_MENU_VERSION_SP_PAUSE"_joaat, false, 2);
				fiber::current()->sleep(200ms);
				CPlayerListMenu* Menu = new CPlayerListMenu();
				u32 Hash{ 0xDA4858C1 };
				u64 Info{ pointers::g_getFriendsMenu(0) };
				u8* Data{ reinterpret_cast<u8*>(Info + 0x8) };
				if (Data) {
					u8 Idx{};
					while (*Data <= 3u) {
						if (*Data == 3) {
							break;
						}
						++Idx;
						Data += 0x10;
					}
					if (Idx < 20ui8) {
						u64 OriginalRID{ *(u64*)(Info + 16ui64 * Idx) };
						*(u64*)(Info + 16ui64 * Idx) = rid;
						pointers::g_triggerPlayermenuAction(Menu, &Hash);
						fiber::current()->sleep(400ms);
						*(u64*)(Info + 16ui64 * Idx) = OriginalRID;
					}
				}
			});
		}
		void forceHostOfScript(hashCommand* command) {
			if (GtaThread* thread{ util::classes::getGtaThread(command->get_hash()) }) {
				if (CGameScriptHandlerNetComponent* netComponet{ util::classes::getScriptHandlerNetComponet(thread) }) {
					if (!netComponet->force_host()) {
						LOG(Info, "Failed to force host of script {}", thread->m_name);
					}
				}
				else {
					LOG(Info, "Script '{}' does not have a valid net componet", thread->m_name);
				}
			}
			else {
				LOG(Info, "Script '{}' does not exist!", command->get_key());
			}
		}
		void forceScriptHost(actionCommand* command) {
			g_engine.primitiveExecute("forceHostOfScript freemode");
		}
		void bail(actionCommand* command) {
			NETWORK::NETWORK_BAIL(0, 0, 16);
			NETWORK::NETWORK_BAIL_TRANSITION(0, 0, 16);
		}
	}
	namespace protections {
		namespace kicks {
			namespace reactions {
				void kick(toggleCommand* command) {

				}
				void crash(toggleCommand* command) {

				}
			}
			void allKickProtections(sectionProtectionCommand* command) {
				"desyncKickProtection"_PC->setFromSection(command->state());
				"lostConnectionKickProtection"_PC->setFromSection(command->state());
				"bailKickProtection"_PC->setFromSection(command->state());
				"nullFunctionKickProtection"_PC->setFromSection(command->state());
				"arrayOverrunKickProtection"_PC->setFromSection(command->state());
			}
		}
		namespace crashes {
			namespace reactions {
				void kick(toggleCommand* command) {

				}
				void crash(toggleCommand* command) {

				}
			}
			void allCrashProtections(sectionProtectionCommand* command) {
				"invalidRemoveWeaponCrashProtection"_PC->setFromSection(command->state());
				"invalidObjectTypeCrashProtection"_PC->setFromSection(command->state());
				"mismatchObjectTypeCrashProtection"_PC->setFromSection(command->state());
				"invalidModelLODCrashProtection"_PC->setFromSection(command->state());
				"invalidModelCrashProtection"_PC->setFromSection(command->state());
				"invalidModelVehicleCrashProtection"_PC->setFromSection(command->state());
				"invalidModelPedCrashProtection"_PC->setFromSection(command->state());
				"invalidSectorPositionCrashProtection"_PC->setFromSection(command->state());
				"vehicleTempActionCrashProtection"_PC->setFromSection(command->state());
				"invalidTempActionCrashProtection"_PC->setFromSection(command->state());
				"invalidTrainTrackIndexCrashProtection"_PC->setFromSection(command->state());
				"pedAttachCrashProtection"_PC->setFromSection(command->state());
				"giveControlCrashProtection"_PC->setFromSection(command->state());
				"ropeCrashProtection"_PC->setFromSection(command->state());
				"invalidWordStateCrashProection"_PC->setFromSection(command->state());
				"pedMountFlagCrashProtection"_PC->setFromSection(command->state());
				"pedStateGadgetOverflowCrashProtection"_PC->setFromSection(command->state());
				"invalidGadgetCrashProtection"_PC->setFromSection(command->state());
				"submarineCarCrashProtection"_PC->setFromSection(command->state());
				"scriptEventIslandHeliLaunchCrashProtection"_PC->setFromSection(command->state());
				"scriptEventRequestVehicleSpawnCrashProtection"_PC->setFromSection(command->state());
				"scriptEventSyncedIntractionCrashProtection"_PC->setFromSection(command->state());
			}
		}
		namespace networkEvents {
			namespace reactions {
				void kick(toggleCommand* command) {

				}
				void crash(toggleCommand* command) {

				}
			}
			void allNetworkEventProtections(sectionProtectionCommand* command) {
				"freezeProtection"_PC->setFromSection(command->state());
				"ragdollProtection"_PC->setFromSection(command->state());
				"requestControlProtection"_PC->setFromSection(command->state());
				"giveWeaponProtection"_PC->setFromSection(command->state());
				"removeWeaponProtection"_PC->setFromSection(command->state());
				"removeAllWeaponsProtection"_PC->setFromSection(command->state());
				"voteKickProtection"_PC->setFromSection(command->state());
			}
		}
		namespace scriptEvents {
			namespace reactions {
				void redirect(toggleCommand* command) {

				}
				void kick(toggleCommand* command) {

				}
				void crash(toggleCommand* command) {

				}
			}
			void allScriptEventProtections(sectionProtectionCommand* command) {
				"ceoBanProtection"_PC->setFromSection(command->state());
				"ceoKickProtection"_PC->setFromSection(command->state());
				"ceoMoneyProtection"_PC->setFromSection(command->state());
				"clearWantedLevelProtection"_PC->setFromSection(command->state());
				"forceMissionProtection"_PC->setFromSection(command->state());
				"giveCollectableProtection"_PC->setFromSection(command->state());
				"gtaBannerProtection"_PC->setFromSection(command->state());
				"personalVehicleDestroyedProtection"_PC->setFromSection(command->state());
				"remoteOffRadarProtection"_PC->setFromSection(command->state());
				"sendToCutsceneProtection"_PC->setFromSection(command->state());
				"sendToIslandProtection"_PC->setFromSection(command->state());
				"sendToLocationProtection"_PC->setFromSection(command->state());
				"soundSpamProtection"_PC->setFromSection(command->state());
				"spectateProtection"_PC->setFromSection(command->state());
				"teleportProtection"_PC->setFromSection(command->state());
				"teleportToWarehouseProtection"_PC->setFromSection(command->state());
				"vehicleKickProtection"_PC->setFromSection(command->state());
				"mcTeleportProtection"_PC->setFromSection(command->state());
				"startActivityProtection"_PC->setFromSection(command->state());
				"kickFromInteriorProtection"_PC->setFromSection(command->state());
				"sendTextLabelSMSProtection"_PC->setFromSection(command->state());
				"sendTextMessageProtection"_PC->setFromSection(command->state());
				"tseCommandRotateCamProtection"_PC->setFromSection(command->state());
				"notificationProtection"_PC->setFromSection(command->state());
				"customNotificationProtection"_PC->setFromSection(command->state());
				"moneyBankedNotificationProtection"_PC->setFromSection(command->state());
				"moneyStolenNotificationProtection"_PC->setFromSection(command->state());
				"moneyRemovedNotificationProtection"_PC->setFromSection(command->state()); 
				"destoryPersonalVehicleProtection"_PC->setFromSection(command->state());
				"triggerCeoRaidProtection"_PC->setFromSection(command->state());
				"startScriptBeginProtection"_PC->setFromSection(command->state());
				"startScriptProceedProtection"_PC->setFromSection(command->state());
			}
		}
		namespace general {
			namespace reactions {
				void kick(toggleCommand* command) {

				}
				void crash(toggleCommand* command) {

				}
			}
			void allGeneralProtections(sectionProtectionCommand* command) {
				"remoteTeleportProtecton"_PC->setFromSection(command->state());
				"reportProtection"_PC->setFromSection(command->state());
				"chatSpamProtection"_PC->setFromSection(command->state());
			}
		}
	}
	namespace miscellaneous {
		namespace game {
			void mobileRadio(toggleCommand* command) {
				AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(command->get(0).toggle);
			}
			void autoMp(toggleCommand* command) {
				LOADINGSCREEN::LOBBY_SET_AUTO_MULTIPLAYER(command->get(0).toggle);
			}
			void setPrologueComplete(actionCommand* command) {
				STATS::SET_PROFILE_SETTING_PROLOGUE_COMPLETE();
			}
			void exitInstantly(toggleCommand* command) {
				if (HUD::IS_WARNING_MESSAGE_ACTIVE()) {
					switch (HUD::GET_WARNING_SCREEN_MESSAGE_HASH()) {
					case "EXIT_SURE"_joaat: {
						if (command->get(0).toggle) {
							exit(0);
							return;
						}
						if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 201)) {
							exit(0);
						}
					} break;
					case "EXIT_SURE_2"_joaat: {
						if (command->get(0).toggle) {
							exit(0);
							return;
						}
						if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 201)) {
							exit(0);
						}
					} break;
					default: {} break;
					}
				}
			}
			void setTimeScale(floatCommand* command) {
				MISC::SET_TIME_SCALE(command->get(0).floating_point);
			}
		}
		namespace world {
			void teleportToWaypoint(actionCommand* command) {
				util::blip waypoint{ 8, util::eBlipHandleType::First };
				if (waypoint.exists()) {
					Vector3 coords{ waypoint.coords() };
					g_engine.primitiveExecute("teleportToCoords {} {} {}", coords.x, coords.y, coords.z);
				}
			}
		}
	}
	namespace settings {
		namespace game {
			void unload(actionCommand* command) {
				g_running = false;
			}
			void exit(actionCommand* command) {
				if ("exitInstantly"_TC->get(0).toggle) {
					exit(0);
					return;
				}
				g_renderer->m_callbacks.push_back(callback([](bool& active) {
					ONCE_PER_FRAME({ elements::openPopup("Close?"); });
					ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
					elements::setNextWindowPos(center, NULL, { 0.5f, 0.5f });
					elements::setNextWindowSize({ 470.f, 235.f });
					elements::font(g_renderer->m_tahoma, [&] {
						elements::popupModal("Close?", [&] {
							util::onPress(VK_ACCEPT, [&] { abort(); elements::closeCurrentPopup(); active = false; });
							util::onPress(VK_RETURN, [&] { elements::closeCurrentPopup(); active = false; });
							elements::text("Grand Theft Auto V will close.\nAre you sure you want to do this?\n\n");
							elements::separator();
							elements::button("Yes", [&] { abort(); elements::closeCurrentPopup(); active = false; }, { 221.f, 0.f }, true);
							elements::setItemDefaultFocus();
							elements::button("No", [&] { elements::closeCurrentPopup(); active = false; }, { 221.f, 0.f });
						}, ImGuiWindowFlags_NoResize);
					});
				}));
			}
		}
		namespace ui {
			void scale(floatCommand* command) {
				script::g_scale = command->get(0).floating_point;
			}
		}
	}
	void init() {
		//Self::Ped::Proofs
		g_manager.add(actionCommand("setAllProofs", "Set All Proofs", self::ped::proofs::setAllProofs));
		g_manager.add(toggleCommand("bulletProof", "Bullet", self::ped::proofs::bullet));
		g_manager.add(toggleCommand("fireProof", "Fire", self::ped::proofs::fire));
		g_manager.add(toggleCommand("collisionProof", "Collision", self::ped::proofs::collision));
		g_manager.add(toggleCommand("meleeProof", "Melee", self::ped::proofs::melee));
		g_manager.add(toggleCommand("godProof", "God", self::ped::proofs::god));
		g_manager.add(toggleCommand("explosionProof", "Explosion", self::ped::proofs::explosion));
		g_manager.add(toggleCommand("steamProof", "Stream", self::ped::proofs::steam));
		g_manager.add(toggleCommand("drownProof", "Drowm", self::ped::proofs::drown));
		g_manager.add(toggleCommand("waterProof", "Water", self::ped::proofs::water));
		//Self::Ped::Health
		g_manager.add(actionCommand("maxHealth", "Max Amount", self::ped::health::maxAmount));
		//Self::Ped::Armor
		g_manager.add(actionCommand("maxArmor", "Max Amount", self::ped::armor::maxAmount));
		//Self::Ped
		g_manager.add(toggleCommand("tinyPed", "Tiny Ped", self::ped::tinyPed));
		g_manager.add(toggleCommand("autoHeal", "Auto Heal", self::ped::autoHeal));
		g_manager.add(toggleCommand("instantRespawn", "Instant Respawn", self::ped::instantRespawn));
		g_manager.add(toggleCommand("keepLastCoordinatesOnDeath", "Keep Last Coordinates On Death", self::ped::keepLastCoordinatesOnDeath));
		g_manager.add(toggleCommand("infiniteOxygen", "Infinite Oxygen", self::ped::infiniteOxygen));
		g_manager.add(actionCommand("clearTasks", "Clear Tasks", self::ped::clearTasks));
		g_manager.add(actionCommand("clone", "Clone", self::ped::clone));
		g_manager.add(actionCommand("suicide", "Suicide", self::ped::suicide));
		//Self::Movement
		g_manager.add(toggleFloatCommand("run", "Run", self::movement::run));
		g_manager.add(toggleFloatCommand("swim", "Swim", self::movement::swim));
		g_manager.add(toggleFloatCommand("stamina", "Stamina", self::movement::stamina));
		g_manager.add(toggleFloatCommand("staminaRegeneration", "Stamina Regeneration", self::movement::staminaRegen));
		g_manager.add(toggleCommand("gracefulLanding", "Graceful Landing", self::movement::gracefulLanding));
		g_manager.add(toggleCommand("beastJump", "Beast Jump", self::movement::beastJump));
		g_manager.add(toggleCommand("superJump", "Super Jump", self::movement::superJump));
		g_manager.add(toggleCommand("ultraJump", "Ultra Jump", self::movement::ultraJump));
		g_manager.add(toggleFloatCommand("superRun", "Super Run", self::movement::superRun));
		g_manager.add(toggleFloatCommand("noClip", "No Clip", self::movement::noClip));
		g_manager.add(toggleCommand("coordSmash", "Coord Smash", "Builds up momentum by setting you in the air and infinite falling", self::movement::coordSmash));
		g_manager.add(toggleCommand("autoTeleportToWaypoint", "Auto Teleport To Waypoint", self::movement::autoTeleportToWaypoint));
		g_manager.add(toggleCommand("walkOnAir", "Walk On Air", self::movement::walkOnAir));
		//Self::World
		g_manager.add(toggleCommand("walkOnWater", "Walk On Water", "Walk on water, instead of swimming", self::world::walkOnWater));
		g_manager.add(toggleCommand("walkThroughWater", "Walk Through Water", "Allows you to walk right through water", self::world::walkThroughWater));
		g_manager.add(toggleCommand("slowMotion", "Slow Motion", "Time moves slower", self::world::slowMotion));
		g_manager.add(variadicCommand("teleportToCoords", "Teleport To Coords", "Teleports to the specific coordinates", { { eValueType::FloatingPoint }, { eValueType::FloatingPoint }, { eValueType::FloatingPoint } }, self::world::teleportToCoords, false));
		//Self::Police
		g_manager.add(toggleCommand("neverWanted", "Never Wanted", "No police are spawned", self::police::neverWanted));
		g_manager.add(toggleCommand("lockWantedLevel", "Lock Wanted Level", "Stays at the same level when toggled", self::police::lockWantedLevel));
		g_manager.add(intCommand("wantedLevel", "Wanted Level", self::police::wantedLevel, true));
		g_manager.add(intCommand("fakeWantedLevel", "Fake Wanted Level", self::police::fakeWantedLevel, true));
		//Self
		g_manager.add(intCommand("alpha", "Alpha", "Woah, I'm seethrough", self::alpha, true));
		g_manager.add(toggleCommand("noCollision", "No Collision", "Phase through objects like a ghost", self::noCollision));
		g_manager.add(toggleCommand("noRagdoll", "No Ragdoll", "No more flinging across the map for you", self::noRagdoll));
		g_manager.add(toggleCommand("invisibility", "Invisibility", "Poof goes your player", self::invisibility));
		//Weapon::Ammo::Special
		g_manager.add(toggleCommand("explosiveAmmo", "Exposive", "Explosive Ammo", weapon::ammo::special::explosive));
		//Weapon::Ammo
		g_manager.add(toggleCommand("infiniteAmmo", "Infinite Ammo", "Pretty self explanitory", weapon::ammo::infinite));
		g_manager.add(toggleCommand("infiniteClip", "Infinite Clip", "Makes it so you never reload", weapon::ammo::infiniteClip));
		g_manager.add(toggleCommand("infiniteStickyBombs", "Infinite Sticky Bombs", "Removes the limit on C4s", weapon::ammo::infiniteStickyBombs));
		g_manager.add(toggleCommand("infiniteFlares", "Infinite Flares", "Removes the limit on flares", weapon::ammo::infiniteFlares));
		g_manager.add(actionCommand("refillAmmo", "Refill", "Refills all your ammo", weapon::ammo::refill));
		//Weapon
		g_manager.add(toggleCommand("teleportGun", "Teleport Gun", &weapon::teleportGun));
		g_manager.add(toggleCommand("deleteGun", "Delete Gun", &weapon::deleteGun));
		g_manager.add(toggleCommand("deadEye", "Dead Eye", &weapon::deadEye));
		//Vehicle::Spawner
		g_manager.add(toggleCommand("spawnInsideVehicle", "Spawn Inside Vehicle", &vehicle::spawner::spawnInsideVehicle));
		g_manager.add(toggleCommand("spawnVehicleMaxed", "Spawn Vehicle Maxed", &vehicle::spawner::spawnVehicleMaxed));
		g_manager.add(toggleCommand("deleteLastSpawnedVehicle", "Delete Last Spawned Vehicle", &vehicle::spawner::deleteLastSpawnedVehicle));
		g_manager.add(hashCommand("spawnVehicle", "Spawn Vehicle", &vehicle::spawner::spawnVehicle));
		//Network::Friends::Selected
		g_manager.add(actionCommand("copyFriendRid", "Copy Friend RID", "Copies their RID to your clipboard", network::friends::selected::copyRid));
		g_manager.add(actionCommand("copyFriendName", "Copy Friend Name", "Copies their name to your clipboard", network::friends::selected::copyName));
		g_manager.add(actionCommand("joinFriend", "Join Friend", "Joins their session if possible", network::friends::selected::join));
		//Network::Friends
		g_manager.add(stringCommand("removeFriend", "Remove Friend", "Sends a SCAPI request to remove a friend", network::friends::remove));
		//Network::Tunables
		g_manager.add(toggleCommand("offRadar", "Off Radar", "Disappear like thin air", network::tunables::offRadar));
		//Network::Session::Browser
		g_manager.add(toggleIntCommand("matchmakingRegion", "Matchmaking Region", network::session::browser::matchmakingRegion));
		g_manager.add(toggleIntCommand("matchmakingLanguage", "Matchmaking Language", network::session::browser::matchmakingLanguage));
		g_manager.add(intCommand("matchmakingPlayerCountMinimum", "Matchmaking Player Count Minimum", network::session::browser::matchmakingPlayerCountMinimum));
		g_manager.add(intCommand("matchmakingPlayerCountMaximum", "Matchmaking Player Count Maximum", network::session::browser::matchmakingPlayerCountMaximum));
		g_manager.add(toggleIntCommand("matchmakingType", "Matchmaking Type", network::session::browser::matchmakingType));
		//Network::Session::Starter
		g_manager.add(stringCommand("go", "Session Starter", "Join a session", network::session::starter::go));
		g_manager.add(actionCommand("leave", "Leave", "Leave from online", network::session::starter::leave));
		g_manager.add(toggleCommand("seamlessTransition", "Seamless Transition", "Seamlessly join a session", network::session::starter::seamlessTransition));
		//Network::Socialclub
		g_manager.add(stringCommand("nameToRid", "Name To Rockstar ID", "Converts a given name to an RID and copies it to clipboard", network::socialclub::nameToRid));
		g_manager.add(variadicCommand("ridToName", "Rockstar ID To Name", "Converts a given RID to an name and copies it to clipboard", { { eValueType::UInt64 } }, network::socialclub::ridToName, false));
		g_manager.add(variadicCommand("scMessage", "Message", "Messages them on Socialclub", { { eValueType::String }, { eValueType::String } }, network::socialclub::scMessage, false));
		g_manager.add(stringCommand("convert", "Convert", "Converts a given name or RID to their counterpart and copies it to clipboard", network::socialclub::convert));
		//Network
		g_manager.add(stringCommand("join", "Join", "Join a player", network::join));
		g_manager.add(hashCommand("forceHostOfScript", "Force Host Of Script", "Force host of an script", network::forceHostOfScript));
		g_manager.add(actionCommand("forceScriptHost", "Force Script Host", "Foces host of the script 'freemode'", network::forceScriptHost));
		g_manager.add(actionCommand("bail", "Bail", "Bail from online", network::bail));
		//Protections::Kicks
		g_manager.add(sectionProtectionCommand("allKickProtections", "All Kick Protections", "Sets all kick protections", protections::kicks::allKickProtections));
		g_manager.add(protectionCommand("desyncKickProtection", "Desync", "May cause an modder detction"));
		g_manager.add(protectionCommand("lostConnectionKickProtection", "Lost Connection"));
		g_manager.add(protectionCommand("bailKickProtection", "Bail"));
		g_manager.add(protectionCommand("nullFunctionKickProtection", "Null Function"));
		g_manager.add(protectionCommand("arrayOverrunKickProtection", "ScriptVM Overrun Kick", "This protects against all script event kicks that cause an array overflow"));
		//Protections::Crashes
		g_manager.add(sectionProtectionCommand("allCrashProtections", "All Crash Protections", "Sets all crash protections", protections::crashes::allCrashProtections));
		g_manager.add(protectionCommand("invalidRemoveWeaponCrashProtection", "Invalid Weapon Remove"));
		g_manager.add(protectionCommand("invalidObjectTypeCrashProtection", "Invalid Object Type"));
		g_manager.add(protectionCommand("mismatchObjectTypeCrashProtection", "Mismatch Object Type"));
		g_manager.add(protectionCommand("invalidModelLODCrashProtection", "Invalid LOD Model"));
		g_manager.add(protectionCommand("invalidModelCrashProtection", "Invalid Model"));
		g_manager.add(protectionCommand("invalidModelVehicleCrashProtection", "Invalid Vehicle Model"));
		g_manager.add(protectionCommand("invalidModelPedCrashProtection", "Invalid Ped Model"));
		g_manager.add(protectionCommand("invalidSectorPositionCrashProtection", "Invalid Sector Position"));
		g_manager.add(protectionCommand("vehicleTempActionCrashProtection", "Vehicle Temp Action"));
		g_manager.add(protectionCommand("invalidTempActionCrashProtection", "Invalid Temp Action"));
		g_manager.add(protectionCommand("invalidTrainTrackIndexCrashProtection", "Invalid Train Track Index"));
		g_manager.add(protectionCommand("pedAttachCrashProtection", "Infinite Ped Attachment"));
		g_manager.add(protectionCommand("giveControlCrashProtection", "Invalid Give Control Type"));
		g_manager.add(protectionCommand("ropeCrashProtection", "Invalid Rope Crash"));
		g_manager.add(protectionCommand("invalidWordStateCrashProection", "Invalid World State"));
		g_manager.add(protectionCommand("pedMountFlagCrashProtection", "Ped Mount Flag"));
		g_manager.add(protectionCommand("pedStateGadgetOverflowCrashProtection", "Ped State Gadget Overflow"));
		g_manager.add(protectionCommand("invalidGadgetCrashProtection", "Invalid Gadget"));
		g_manager.add(protectionCommand("submarineCarCrashProtection", "Submarine Car"));
		g_manager.add(protectionCommand("scriptEventIslandHeliLaunchCrashProtection", "Script Event Island Heli Launch"));
		g_manager.add(protectionCommand("scriptEventRequestVehicleSpawnCrashProtection", "Script Event Request Vehicle Spawn"));
		g_manager.add(protectionCommand("scriptEventSyncedIntractionCrashProtection", "Script Event Synced Interaction"));
		//Protections::Events::Network
		g_manager.add(sectionProtectionCommand("allNetworkEventProtections", "All Network Event Protections", "Sets all network event protections", protections::networkEvents::allNetworkEventProtections));
		g_manager.add(protectionCommand("freezeProtection", "Freeze"));
		g_manager.add(protectionCommand("ragdollProtection", "Ragdoll"));
		g_manager.add(protectionCommand("requestControlProtection", "Request Control"));
		g_manager.add(protectionCommand("giveWeaponProtection", "Give Weapon"));
		g_manager.add(protectionCommand("removeWeaponProtection", "Remove Weapon"));
		g_manager.add(protectionCommand("removeAllWeaponsProtection", "Remove All Weapons"));
		g_manager.add(protectionCommand("voteKickProtection", "Vote Kick"));
		//Protections::Events::Script
		g_manager.add(sectionProtectionCommand("allScriptEventProtections", "All Network Event Protections", "Sets all script event protections", protections::scriptEvents::allScriptEventProtections));
		g_manager.add(protectionCommand("ceoBanProtection", "CEO Ban"));
		g_manager.add(protectionCommand("ceoKickProtection", "CEO Kick"));
		g_manager.add(protectionCommand("ceoMoneyProtection", "CEO Money"));
		g_manager.add(protectionCommand("clearWantedLevelProtection", "Clear Wanted Level"));
		g_manager.add(protectionCommand("forceMissionProtection", "Force Mission"));
		g_manager.add(protectionCommand("giveCollectableProtection", "Give Collectable"));
		g_manager.add(protectionCommand("gtaBannerProtection", "GTA Banner"));
		g_manager.add(protectionCommand("personalVehicleDestroyedProtection", "Personal Vehicle Destroyed"));
		g_manager.add(protectionCommand("remoteOffRadarProtection", "Remote Off-Radar"));
		g_manager.add(protectionCommand("sendToCutsceneProtection", "Send To Cutscene"));
		g_manager.add(protectionCommand("sendToIslandProtection", "Send To Island"));
		g_manager.add(protectionCommand("sendToLocationProtection", "Send To Location"));
		g_manager.add(protectionCommand("soundSpamProtection", "Sound Spam"));
		g_manager.add(protectionCommand("spectateProtection", "Spectate"));
		g_manager.add(protectionCommand("teleportProtection", "Teleport"));
		g_manager.add(protectionCommand("teleportToWarehouseProtection", "Teleport To Warehouse"));
		g_manager.add(protectionCommand("vehicleKickProtection", "Vehicle Kick"));
		g_manager.add(protectionCommand("mcTeleportProtection", "MC Teleport"));
		g_manager.add(protectionCommand("startActivityProtection", "Start Activity"));
		g_manager.add(protectionCommand("kickFromInteriorProtection", "Kick From Interior"));
		g_manager.add(protectionCommand("sendTextLabelSMSProtection", "Send Text Label SMS"));
		g_manager.add(protectionCommand("sendTextMessageProtection", "Send Text Message"));
		g_manager.add(protectionCommand("tseCommandRotateCamProtection", "TSE Command Rotate Cam"));
		g_manager.add(protectionCommand("notificationProtection", "Notification"));
		g_manager.add(protectionCommand("customNotificationProtection", "Custom Notification"));
		g_manager.add(protectionCommand("moneyBankedNotificationProtection", "Money Banked Notification"));
		g_manager.add(protectionCommand("moneyStolenNotificationProtection", "Money Stolen Notification"));
		g_manager.add(protectionCommand("moneyRemovedNotificationProtection", "Money Removed Notification"));
		g_manager.add(protectionCommand("destoryPersonalVehicleProtection", "Destroy Personal Vehicle"));
		g_manager.add(protectionCommand("triggerCeoRaidProtection", "Trigger CEO Raid"));
		g_manager.add(protectionCommand("startScriptBeginProtection", "Start Script Begin"));
		g_manager.add(protectionCommand("startScriptProceedProtection", "Start Script Proceed"));
		//Protections
		g_manager.add(sectionProtectionCommand("allGeneralProtections", "All General Protections", "Sets general protections", protections::general::allGeneralProtections));
		g_manager.add(protectionCommand("remoteTeleportProtecton", "Remote Teleport"));
		g_manager.add(protectionCommand("reportProtection", "Reports"));
		g_manager.add(protectionCommand("chatSpamProtection", "Chat Spam", "Stops chat spam from being displayed"));
		//Miscellaneous::Game
		g_manager.add(toggleCommand("mobileRadio", "Mobile Radio", "Use the game's radio anywhere", miscellaneous::game::mobileRadio));
		g_manager.add(toggleCommand("autoMp", "Auto Multiplayer", "Automatically load into multiplayer on startup", miscellaneous::game::autoMp));
		g_manager.add(actionCommand("setPrologueComplete", "Set Prologue Complete", miscellaneous::game::setPrologueComplete));
		g_manager.add(toggleCommand("exitInstantly", "Exit Instantly", "Exits the game instantly when exit is requested", miscellaneous::game::exitInstantly));
		g_manager.add(floatCommand("setTimeScale", "Set Time Scale", miscellaneous::game::setTimeScale));
		//Miscellaneous::World
		g_manager.add(actionCommand("teleportToWaypoint", "Teleport To Waypoint", miscellaneous::world::teleportToWaypoint));
		//Settings::Ui
		g_manager.add(floatCommand("scale", "Scale", "Sets the global UI scale", settings::ui::scale, true));
		//Settings::Game
		g_manager.add(actionCommand("unload", "Unload", "Removes " BRAND " from the game", settings::game::unload));
		g_manager.add(actionCommand("exit", "Exit", "Exit the game", settings::game::exit));
	}
	void uninit() {
		Ped ped{ PLAYER::PLAYER_PED_ID() };
		ENTITY::RESET_ENTITY_ALPHA(ped);
		ENTITY::SET_ENTITY_VISIBLE(ped, TRUE, FALSE);
		ENTITY::SET_ENTITY_HAS_GRAVITY(ped, TRUE);
		PLAYER::SET_MAX_WANTED_LEVEL(5);
		PLAYER::SET_POLICE_RADAR_BLIPS(TRUE);
		Player player{ PLAYER::PLAYER_ID() };
		PLAYER::RESET_WANTED_LEVEL_HIDDEN_ESCAPE_TIME(player);
	}
	void onInit() {
		//These need to be after init because the values aren't created yet
		//Self::Movement
		"run"_TFC->get(1).floating_point = 1.f;
		"swim"_TFC->get(1).floating_point = 1.f;
		"stamina"_TFC->get(1).floating_point = 11.f;
		"staminaRegeneration"_TFC->get(1).floating_point = 1.f;
		"superRun"_TFC->get(1).floating_point = 2.f;
		"noClip"_TFC->get(1).floating_point = 1.f;
		"coordSmash"_TC->add_hotkey(VK_F10);
		//Self
		"alpha"_IC->get(0).i32 = 255;
		//Network::Session::Browser
		"matchmakingPlayerCountMinimum"_IC->get(0).i32 = 1;
		"matchmakingPlayerCountMaximum"_IC->get(0).i32 = 32;
		//Settings::Ui
		"scale"_FC->get(0).floating_point = 1.f;
	}
	void onTick() {
		cPed = util::classes::getPed();
		if (cPed)
			cPedWeaponManager = cPed->m_weapon_manager;
		if (cPedWeaponManager)
			cWeaponInfo = cPedWeaponManager->m_weapon_info;
		if (cWeaponInfo)
			cAmmoInfo = cWeaponInfo->m_ammo_info;
		if (cPed)
			cVehicle = cPed->m_vehicle;
		if (cPed)
			cPlayerInfo = cPed->m_player_info;
	}
}