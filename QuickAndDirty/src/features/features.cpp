#include "features.h"
#include "script/script.h"

namespace features {
	namespace self {
		namespace ped {
			namespace proofs {
				void bullet(toggleFeature* feature) {
					cPed->set_damage_bit(eEntityProofs::Bullet, feature->v().toggle);
				}
				void fire(toggleFeature* feature) {
					cPed->set_damage_bit(eEntityProofs::Fire, feature->v().toggle);
				}
				void collision(toggleFeature* feature) {
					cPed->set_damage_bit(eEntityProofs::Collision, feature->v().toggle);
				}
				void melee(toggleFeature* feature) {
					cPed->set_damage_bit(eEntityProofs::Melee, feature->v().toggle);
				}
				void god(toggleFeature* feature) {
					cPed->set_damage_bit(eEntityProofs::God, feature->v().toggle);
				}
				void explosion(toggleFeature* feature) {
					cPed->set_damage_bit(eEntityProofs::Explosion, feature->v().toggle);
				}
				void steam(toggleFeature* feature) {
					cPed->set_damage_bit(eEntityProofs::Steam, feature->v().toggle);
				}
				void drown(toggleFeature* feature) {
					cPed->set_damage_bit(eEntityProofs::Drown, feature->v().toggle);
				}
				void water(toggleFeature* feature) {
					cPed->set_damage_bit(eEntityProofs::Water, feature->v().toggle);
				}
			}
			namespace health {
				void maxAmount(features::actionFeature* feature) {
					cPed->m_health = cPed->m_maxhealth;
				}
			}
			namespace armor {
				void maxAmount(features::actionFeature* feature) {
					cPed->m_armor = 255.f;
				}
			}
			void suicide(features::actionFeature* feature) {
				cPed->m_health = 0.f;
			}
		}
		namespace movement {
			void run(toggleFloatFeature* feature) {
				cPlayerInfo->m_run_speed = feature->v().toggle ? feature->get(1).floating_point : 1.f;
			}
			void swim(toggleFloatFeature* feature) {
				cPlayerInfo->m_swim_speed = feature->v().toggle ? feature->get(1).floating_point : 1.f;
			}
			void stamina(toggleFloatFeature* feature) {
				cPlayerInfo->m_stamina = feature->v().toggle ? feature->get(1).floating_point : 11.f;
			}
			void staminaRegen(toggleFloatFeature* feature) {
				cPlayerInfo->m_stamina_regen = feature->v().toggle ? feature->get(1).floating_point : 1.f;
			}
			void beastJump(toggleFeature* feature) {

			}
			void superJump(toggleFeature* feature) {

			}
			void gracefulLanding(toggleFeature* feature) {

			}
			void superRun(toggleFloatFeature* feature) {
				if (feature->v().toggle) {
					if (PAD::IS_DISABLED_CONTROL_PRESSED(2, 32) && PAD::IS_DISABLED_CONTROL_PRESSED(2, 21)) {
						Player player{ PLAYER::PLAYER_ID() };
						Ped ped{ PLAYER::PLAYER_PED_ID() };
						ENTITY::APPLY_FORCE_TO_ENTITY(ped, 1, { 0.f, feature->get(1).floating_point, 0.f }, {}, 0, TRUE, TRUE, TRUE, FALSE, FALSE);
						PLAYER::SET_PLAYER_SPRINT(player, true);
						PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 5.f);
					}
				}
			}
			void noClip(toggleFloatFeature* feature) {
				Ped ped{ PLAYER::PLAYER_PED_ID() };
				ENTITY::SET_ENTITY_COLLISION(ped, !feature->v().toggle, FALSE);
				if (feature->v().toggle) {
					Vector3 pos{ cPed->get_position().serialize() };
					Vector3 rot{ math::rotToDir(CAM::GET_GAMEPLAY_CAM_ROT(0)) };
					Vector3 camCoords{ pos + rot * feature->get(1).floating_point };
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, pos, true, true, true);
					if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 32)) {
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, camCoords, true, true, true);
					}
				}
			}
		}
		namespace police {
			void neverWanted(toggleFeature* feature) {
				Player id{ PLAYER::PLAYER_ID() };
				if (feature->v().toggle) {
					cPlayerInfo->m_wanted_level = 0;
					PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(id, TRUE);
				}
				PLAYER::SET_DISPATCH_COPS_FOR_PLAYER(id, feature->v().toggle);
			}
			void wantedLevel(intFeature* feature) {
				cPlayerInfo->m_wanted_level_display = cPlayerInfo->m_wanted_level = feature->v().i32;
				cPlayerInfo->m_is_wanted = cPlayerInfo->m_wanted_level > 0;
			}
		}
		void alpha(intFeature* feature) {
			Ped ped{ PLAYER::PLAYER_PED_ID() };
			ENTITY::SET_ENTITY_ALPHA(ped, feature->v().toggle, FALSE);
		}
		void invisibility(toggleFeature* feature) {
			if (feature->v().toggle) {
				cPed->m_entity_flags &= ~(uint32_t)eEntityFlags::Visible;
			}
			else {
				cPed->m_entity_flags |= (uint32_t)eEntityFlags::Visible;
			}
		}
		void noRagdoll(toggleFeature* feature) {
			Ped ped{ PLAYER::PLAYER_PED_ID() };
			if (feature->v().toggle) {
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
				void explosive(toggleFeature* feature) {
					if (cAmmoInfo) {
						if (feature->v().toggle) {
							cAmmoInfo->m_ammo_special_type |= (uint32_t)eAmmoSpecialType::Explosive;
						}
						else {
							cAmmoInfo->m_ammo_special_type &= ~(uint32_t)eAmmoSpecialType::Explosive;
						}
					}
				}
			}
			void infinite(toggleFeature* feature) {
				cPed->m_inventory->m_infinite_ammo = feature->v().toggle;
			}
			void infiniteClip(toggleFeature* feature) {
				cPed->m_inventory->m_infinite_clip = feature->v().toggle;
			}
			void refill(actionFeature* feature) {
				Ped ped{ PLAYER::PLAYER_PED_ID() };
				WEAPON::REFILL_AMMO_INSTANTLY(ped);
			}
		}
	}
	namespace vehicle {
		namespace spawner {

		}
	}
	namespace network {
		namespace friends {
			namespace selected {
				void copyRid(actionFeature* feature) {
					static FriendInfo& info{ util::network::friends::getFriends()[g_selectedFriend] };
					features::cmd::g_engine.primitiveExecute("copyText {}", info.m_rockstar_id);
				}
				void copyName(actionFeature* feature) {
					static FriendInfo& info{ util::network::friends::getFriends()[g_selectedFriend] };
					features::cmd::g_engine.primitiveExecute("copyText {}", info.m_name);
				}
			}
			void remove(variadicFeature* feature) {

			}
		}		
		namespace session {
			namespace players {
				namespace selected {

				}
			}
			namespace starter {
				void go(variadicFeature* feature) {
					static tables::eSessionType type{};
					std::string cid{ feature->get(0).string };
					if (isNumber(cid)) {
						type = (tables::eSessionType)stoi(cid);
					}
					else {
						bool found{};
						for (int i{}; i != 11; ++i) {
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
							LOG(FOREGROUND_WHITE, "Commands", "Command 'go' accepts a string but there is no type '{}'", cid);
							return;
						}
					}
					g_fiberPool.add([] {
						global(2695915).value()->Int = type == tables::eSessionType::SCTV;
						if (type == tables::eSessionType::Leave) {
							global(1574589).at(2).value()->Int = -1;
						}
						else {
							global(1575017).value()->Int = (int)type;
						}
						global(1574589).value()->Int = true;
						fiber::current()->sleep(200ms);
						global(1574589).value()->Int = false;
					});
				}
				void leave(actionFeature* feature) {
					features::cmd::g_engine.primitiveExecute("go -1");
					if (!NETWORK::NETWORK_IS_TRANSITION_STARTED() && !NETWORK::NETWORK_IS_IN_TRANSITION()) {
						features::cmd::g_engine.primitiveExecute("bail");
					}
				}
			}
		}
		namespace socialclub {
			namespace backend {
				nlohmann::json jRequest(nlohmann::json body, std::string endpoint) {
					curlWrapper curl{};
					std::vector<std::string> headers = {
						"X-Requested-With: XMLHttpRequest",
						"Content-Type: application/json; charset=utf-8",
						util::network::socialclub::authorizationHeader()
					};
					std::string response{ curl.post(endpoint, body.dump(), headers) };
					nlohmann::json j{ nlohmann::json::parse(response) };
					if (j["Status"]) {
						return j;
					}
					return {};
				}
				bool isOnline(uint64_t rid) {
					if (nlohmann::json json{ jRequest({ { "RockstarId", std::to_string(rid) } }, "https://scui.rockstargames.com/api/friend/getprofile") }; !json.is_null()) {
						for (auto& acc : json["Accounts"]) {
							if (auto& r_acc = acc["RockstarAccount"]; !r_acc.is_null()) {
								if (rid == r_acc["RockstarId"].get<uint64_t>()) {
									return r_acc["IsOnline"].get<bool>();
								}
							}
						}
					}
					return false;
				}
				uint64_t nameToRid(std::string name) {
					if (nlohmann::json json{ jRequest({ { "searchNickname", name } }, "https://scui.rockstargames.com/api/friend/accountsearch") }; !json.is_null()) {
						if (name.size() <= 20) {
							if (auto numAccs = json["Total"].get<int>(); numAccs > 0) {
								return json["Accounts"][0]["RockstarId"].get<uint64_t>();
							}
							else {
								LOG(FOREGROUND_WHITE, "Info", "{} wasn't found. Please ensure there are no spelling mistakes", name);
							}
						}
						else {
							LOG(FOREGROUND_WHITE, "Info", "The character count cannot exceed 20, please shorten the value");
						}
						return 0;
					}
					return 0;
				}
				std::string ridToName(uint64_t rid) {
					if (nlohmann::json json{ jRequest({ { "RockstarId", std::to_string(rid) } }, "https://scui.rockstargames.com/api/friend/getprofile") }; !json.is_null()) {
						for (auto& acc : json["Accounts"]) {
							if (auto& r_acc = acc["RockstarAccount"]; !r_acc.is_null()) {
								if (rid == r_acc["RockstarId"].get<uint64_t>()) {
									return r_acc["Name"].get<std::string>();
								}
							}
						}
					}
					return {};
				}
			}
			void ridToName(variadicFeature* feature) {
				cmd::g_engine.primitiveExecute("copyText {}", backend::ridToName(feature->v().u64));
			}
			void nameToRid(variadicFeature* feature) {
				cmd::g_engine.primitiveExecute("copyText {}", backend::nameToRid(feature->v().string));
			}
		}
		namespace tunables {
			void offRadar(toggleFeature* feature) {
				global(2657589).at(PLAYER::PLAYER_ID(), 466).at(210).value()->Int = feature->v().toggle;
				global(2672505).at(57).value()->Int = NETWORK::GET_NETWORK_TIME() + (feature->v().toggle ? 0xB8E10 : NULL);
			}
		}
		void bail(actionFeature* feature) {
			NETWORK::NETWORK_BAIL(0, 0, 16);
		}
	}
	namespace settings {
		namespace game {
			void autoMp(toggleFeature* feature) {
				LOADINGSCREEN::LOBBY_SET_AUTO_MULTIPLAYER(feature->get(0).toggle);
			}
			void unload(actionFeature* feature) {
				g_running = false;
			}
			void exit(actionFeature* feature) {
				exit(0);
			}
		}
		namespace ui {
			void scale(floatFeature* feature) {
				script::g_scale = feature->v().floating_point;
			}
		}
	}
	namespace commands {
		void copyText(variadicFeature* feature) {
			util::clipboard clipboard{ feature->get(0).string };
			clipboard.set();
		}
	}
	void onTick() {
		//Self::Ped::Proofs
		features::g_manager.add(features::toggleFeature("bulletProof", "Bullet", self::ped::proofs::bullet));
		features::g_manager.add(features::toggleFeature("fireProof", "Fire", self::ped::proofs::fire));
		features::g_manager.add(features::toggleFeature("collisionProof", "Collision", self::ped::proofs::collision));
		features::g_manager.add(features::toggleFeature("meleeProof", "Melee", self::ped::proofs::melee));
		features::g_manager.add(features::toggleFeature("godProof", "God", self::ped::proofs::god));
		features::g_manager.add(features::toggleFeature("explosionProof", "Explosion", self::ped::proofs::explosion));
		features::g_manager.add(features::toggleFeature("steamProof", "Stream", self::ped::proofs::steam));
		features::g_manager.add(features::toggleFeature("drownProof", "Drowm", self::ped::proofs::drown));
		features::g_manager.add(features::toggleFeature("waterProof", "Water", self::ped::proofs::water));
		//Self::Ped::Health
		features::g_manager.add(features::actionFeature("maxHealth", "Max Amount", self::ped::health::maxAmount));
		//Self::Ped::Armor
		features::g_manager.add(features::actionFeature("maxArmor", "Max Amount", self::ped::armor::maxAmount));
		//Self::Ped
		features::g_manager.add(features::actionFeature("suicide", "Suicide", self::ped::suicide));
		//Self::Movement
		features::g_manager.add(features::toggleFloatFeature("run", "Run", self::movement::run));
		features::g_manager.add(features::toggleFloatFeature("swim", "Swim", self::movement::swim));
		features::g_manager.add(features::toggleFloatFeature("stamina", "Stamina", self::movement::stamina));
		features::g_manager.add(features::toggleFloatFeature("staminaRegeneration", "Stamina Regeneration", self::movement::staminaRegen));
		features::g_manager.add(features::toggleFeature("beastJump", "Beast Jump", self::movement::beastJump));
		features::g_manager.add(features::toggleFeature("superJump", "Super Jump", self::movement::superJump));
		features::g_manager.add(features::toggleFeature("gracefulLanding", "Graceful Landing", self::movement::gracefulLanding));
		features::g_manager.add(features::toggleFloatFeature("superRun", "Super Run", self::movement::superRun));
		features::g_manager.add(features::toggleFloatFeature("noClip", "No Clip", self::movement::noClip));
		//Self::Police
		features::g_manager.add(features::toggleFeature("neverWanted", "Never Wanted", "No police are spawned", self::police::neverWanted));
		features::g_manager.add(features::intFeature("wantedLevel", "Wanted Level", self::police::wantedLevel, true));
		//Self
		features::g_manager.add(features::intFeature("alpha", "Alpha", "Woah, I'm seethrough", self::alpha, true));
		features::g_manager.add(features::toggleFeature("noRagdoll", "No Ragdoll", "No more flinging across the map for you", self::noRagdoll));
		features::g_manager.add(features::toggleFeature("invisibility", "Invisibility", "Poof goes your player", self::invisibility));
		//Weapon::Ammo::Special
		features::g_manager.add(features::toggleFeature("explosiveAmmo", "Exposive", "Explosive Ammo", weapon::ammo::special::explosive));
		//Weapon::Ammo
		features::g_manager.add(features::toggleFeature("infiniteAmmo", "Infinite Ammo", "Pretty self explanitory", weapon::ammo::infinite));
		features::g_manager.add(features::toggleFeature("infiniteClip", "Infinite Clip", "Makes it so you never reload", weapon::ammo::infiniteClip));
		features::g_manager.add(features::actionFeature("refillAmmo", "Refill", "Refills all your ammo", weapon::ammo::refill));
		//Network::Friends::Selected
		features::g_manager.add(features::actionFeature("copyFriendRid", "Copy RID", "Copies their RID to your clipboard", network::friends::selected::copyRid));
		features::g_manager.add(features::actionFeature("copyFriendName", "Copy Name", "Copies their name to your clipboard", network::friends::selected::copyName));
		//Network::Tunables
		features::g_manager.add(features::toggleFeature("offradar", "Off Radar", "Disappear like thin air", network::tunables::offRadar));
		//Network::Session::Starter
		features::g_manager.add(features::variadicFeature("go", "Session Starter", "Join a session", { { eValueType::String } }, network::session::starter::go, false));
		features::g_manager.add(features::actionFeature("leave", "Leave", "Leave from online", network::session::starter::leave));
		//Network::Socialclub
		features::g_manager.add(features::variadicFeature("nameToRid", "Name To Rockstar ID", "Converts a given name to an RID and copies it to clipboard", { { eValueType::String } }, network::socialclub::nameToRid, false));
		features::g_manager.add(features::variadicFeature("ridToName", "Rockstar ID To Name", "Converts a given RID to an name and copies it to clipboard", { { eValueType::UInt64 } }, network::socialclub::ridToName, false));
		//Network
		features::g_manager.add(features::actionFeature("bail", "Bail", "Bail from online", network::bail));
		//Settings::Ui
		features::g_manager.add(features::floatFeature("scale", "Scale", "Set the global UI scale", settings::ui::scale, true));
		//Settings::Game
		features::g_manager.add(features::toggleFeature("automp", "Auto Multiplayer", "Automatically load into multiplayer on startup", settings::game::autoMp));
		features::g_manager.add(features::actionFeature("unload", "Unload", "Unload the module", settings::game::unload));
		features::g_manager.add(features::actionFeature("exit", "Exit", "Exit the game", settings::game::exit));
		//Commands
		features::g_manager.add(features::variadicFeature("copyText", "Copy Text", "Copies text to clipboard", { { eValueType::String } }, commands::copyText, false));
		features::g_manager.init();
		//These need to be after init because the values aren't created yet.
		//Self::Movement
		"run"_TF->get(1).floating_point = 1.f;
		"swim"_TF->get(1).floating_point = 1.f;
		"stamina"_TF->get(1).floating_point = 11.f;
		"staminaRegeneration"_TF->get(1).floating_point = 1.f;
		"superRun"_TF->get(1).floating_point = 2.f;
		"noClip"_TF->get(1).floating_point = 1.f;
		//Self
		"alpha"_IF->get(0).i32 = 255;
		//Settings::Ui
		"scale"_FF->get(0).floating_point = 1.f;
		while (true) {
			cPed = util::classes::getPed();
			cPedWeaponManager = cPed->m_weapon_manager;
			cWeaponInfo = cPedWeaponManager->m_weapon_info;
			cAmmoInfo = cWeaponInfo->m_ammo_info;
			cVehicle = cPed->m_vehicle;
			cPlayerInfo = util::classes::getPlayerInfo();
			features::g_manager.tick();
			if (HUD::IS_WARNING_MESSAGE_ACTIVE()) {
				switch (HUD::GET_WARNING_SCREEN_MESSAGE_HASH()) {
				case "EXIT_SURE"_joaat: {
					if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 201)) {
						exit(0);
					}
				} break;
				case "EXIT_SURE_2"_joaat: {
					if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 201)) {
						exit(0);
					}
				} break;
				default: {} break;
				}
			}
			fiber::current()->sleep();
		}
	}
}