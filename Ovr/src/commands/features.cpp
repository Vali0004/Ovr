#include "features.h"
#include "script/script.h"
#include "hooking/hooking.h"
#include <renderer/renderer.h>
#include <script/elements.h>

namespace commands::features {
	namespace self {
		namespace ped {
			namespace proofs {
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
					cPed->m_armor = 255.f;
				}
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
			void beastJump(toggleCommand* command) {

			}
			void superJump(toggleCommand* command) {
				Player player{ PLAYER::PLAYER_ID() };
				if (command->get(0).toggle) {
					MISC::SET_SUPER_JUMP_THIS_FRAME(player);
				}
			}
			void gracefulLanding(toggleCommand* command) {

			}
			void superRun(toggleFloatCommand* command) {
				if (command->get(0).toggle) {
					if (PAD::IS_DISABLED_CONTROL_PRESSED(2, 32) && PAD::IS_DISABLED_CONTROL_PRESSED(2, 21)) {
						Player player{ PLAYER::PLAYER_ID() };
						Ped ped{ PLAYER::PLAYER_PED_ID() };
						ENTITY::APPLY_FORCE_TO_ENTITY(ped, 1, { 0.f, command->get(1).floating_point, 0.f }, {}, 0, TRUE, TRUE, TRUE, FALSE, FALSE);
						PLAYER::SET_PLAYER_SPRINT(player, true);
						PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 5.f);
					}
				}
			}
			void noClip(toggleFloatCommand* command) {
				Ped ped{ PLAYER::PLAYER_PED_ID() };
				ENTITY::SET_ENTITY_COLLISION(ped, !command->get(0).toggle, FALSE);
				if (command->get(0).toggle) {
					Vector3 pos{ cPed->get_position().serialize() };
					Vector3 rot{ math::rotToDir(CAM::GET_GAMEPLAY_CAM_ROT(0)) };
					Vector3 camCoords{ pos + rot * command->get(1).floating_point };
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, pos, true, true, true);
					if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 32)) {
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, camCoords, true, true, true);
					}
				}
			}
		}
		namespace police {
			void neverWanted(toggleCommand* command) {
				Player id{ PLAYER::PLAYER_ID() };
				if (command->get(0).toggle) {
					cPlayerInfo->m_wanted_level = 0;
					PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(id, TRUE);
				}
				PLAYER::SET_DISPATCH_COPS_FOR_PLAYER(id, command->get(0).toggle);
			}
			void wantedLevel(intCommand* command) {
				cPlayerInfo->m_wanted_level_display = cPlayerInfo->m_wanted_level = command->get(0).i32;
				cPlayerInfo->m_is_wanted = cPlayerInfo->m_wanted_level > 0;
			}
		}
		void alpha(intCommand* command) {
			Ped ped{ PLAYER::PLAYER_PED_ID() };
			ENTITY::SET_ENTITY_ALPHA(ped, command->get(0).toggle, FALSE);
		}
		void invisibility(toggleCommand* command) {
			if (command->get(0).toggle) {
				cPed->m_entity_flags &= ~(uint32_t)eEntityFlags::Visible;
			}
			else {
				cPed->m_entity_flags |= (uint32_t)eEntityFlags::Visible;
			}
		}
		void noRagdoll(toggleCommand* command) {
			Ped ped{ PLAYER::PLAYER_PED_ID() };
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
	}
	namespace vehicle {
		namespace spawner {

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
			}
			void remove(variadicCommand* command) {
				util::clipboard clipboard{};
				clipboard.get();
				std::string curText{ clipboard.str() };
				g_engine.primitiveExecute("nameToRid {}", command->get(0).string);
				clipboard.get();
				std::string rid{ clipboard.str() };
				clipboard.str() = curText;
				clipboard.set();
				auto res = socialclub::backend::jRequest({ "RockstarId", rid }, "https://scapi.rockstargames.com/friends/remove");
				LOG(Info, "Result: {}", res.dump(4));
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
									if ("matchmakingLanguage"_TIF->get(0).toggle && l != "matchmakingLanguage"_TIF->get(1).i32) {
										s.valid = false;
									}
									if (pc < "matchmakingPlayerCountMinimum"_IF->get(0).i32 || pc > "matchmakingPlayerCountMaximum"_IF->get(0).i32) {
										s.valid = false;
									}
									if ("matchmakingType"_TIF->get(0).toggle && ((d & (1 << 14)) == (1 << 14)) != (bool)"matchmakingType"_TIF->get(1).i32) {
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
				void go(variadicCommand* command) {
					static tables::eSessionType type{};
					std::string cid{ command->get(0).string };
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
							LOG(Commands, "Command 'go' accepts a string but there is no type '{}'", cid);
							return;
						}
					}
					LOG(Info, "Go command called with type {}", (i32)type);
					g_fiberPool.add([] {
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
			}
		}
		namespace socialclub {
			namespace backend {
				nlohmann::json jRequest(nlohmann::json body, std::string endpoint) {
					curlWrapper curl{};
					std::vector<const char*> headers{
						"X-Requested-With: XMLHttpRequest",
						"Content-Type: application/json; charset=utf-8",
						util::network::socialclub::authorizationHeader().c_str()
					};
					std::string response{ curl.post(endpoint, body.dump(), headers) };
					nlohmann::json j{ nlohmann::json::parse(response) };
					if (j["Status"]) {
						return j;
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
					if (nlohmann::json json{ jRequest({ { "searchNickname", name } }, "https://scui.rockstargames.com/api/friend/accountsearch") }; !json.is_null()) {
						if (name.size() <= 20) {
							if (i32 numAccs{ json["Total"].get<i32>() }; numAccs > 0) {
								return json["Accounts"][0]["RockstarId"].get<u64>();
							}
							else {
								LOG(Info, "{} wasn't found. Please ensure there are no spelling mistakes", name);
							}
						}
						else {
							LOG(Info, "The character count cannot exceed 20, please shorten the value");
						}
						return 0;
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
				void getGamerTask(u64 rid, std::function<void(rage::rlSessionByGamerTaskResult&)> onSuccess) {
					rage::rlGamerHandle handles[1]{ { rid } };
					rage::rlSessionByGamerTaskResult result{};
					bool success{};
					rage::rlTaskStatus status{};
					if (pointers::g_getGamerTaskResult(0, handles, SIZEOF(handles), &result, 1, &success, &status)) {
						while (status.m_state == 1) {
							fiber::current()->sleep();
						}
						if (status.m_state == 3 && success) {
							onSuccess(result);
						}
					}
				}
			}
			void ridToName(variadicCommand* command) {
				g_engine.primitiveExecute("copyText {}", backend::ridToName(command->get(0).u64));
			}
			void nameToRid(variadicCommand* command) {
				std::thread t([command] {
					u64 rid{ backend::nameToRid(command->get(0).string) };
					if (rid)
						g_engine.primitiveExecute("copyText {}", rid);
				});
				t.detach();
			}
			void convert(variadicCommand* command) {
				std::string str{ command->get(0).string };
				if (str.empty()) {
					LOG(Info, "Please provide an name or RID");
					return;
				}
				if (!isNumber(str)) {
					u64 rid{ backend::nameToRid(str) };
					if (!rid) {
						LOG(Info, "Failed to get {}'s RID", str);
						return;
					}
					str = std::to_string(rid);
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
				std::string strRid{ command->get(0).string };
				if (strRid.empty()) {
					LOG(Info, "Please provide an name or RID");
					return;
				}
				if (!isNumber(strRid)) {
					u64 rid{ backend::nameToRid(strRid) };
					if (!rid) {
						LOG(Info, "Failed to get {}'s RID", strRid);
						return;
					}
					strRid = std::to_string(rid);
				}
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
				//global(2657589).at(PLAYER::PLAYER_ID(), 466).at(210).value()->Int = command->get(0).toggle;
				//global(2672505).at(57).value()->Int = NETWORK::GET_NETWORK_TIME() + (command->get(0).toggle ? 0xB8E10 : NULL);
			}
		}
		void join(variadicCommand* command) {
			std::string strRid{ command->get(0).string };
			if (strRid.empty()) {
				g_notifications.add("Join", "Please provide an name or RID");
			}
			if (!isNumber(strRid)) {
				u64 rid{ socialclub::backend::nameToRid(strRid) };
				if (!rid) {
					g_notifications.add("Join", "Failed to get {}'s RID", strRid);
				}
				strRid = std::to_string(rid);
			}
			u64 rid{ stoull(strRid) };
			socialclub::backend::getGamerTask(rid, [](rage::rlSessionByGamerTaskResult& result) {
				printf("Gamer task success\n");
			});
		}
		void bail(actionCommand* command) {
			NETWORK::NETWORK_BAIL(0, 0, 16);
			NETWORK::NETWORK_BAIL_TRANSITION(0, 0, 16);
		}
	}
	namespace settings {
		namespace game {
			void autoMp(toggleCommand* command) {
				LOADINGSCREEN::LOBBY_SET_AUTO_MULTIPLAYER(command->get(0).toggle);
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
			void unload(actionCommand* command) {
				g_running = false;
			}
			void exit(actionCommand* command) {
				g_renderer->m_callbacks.push_back(callback([](bool& active) {
					ONCE({ elements::openPopup("Close?"); });
					ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
					elements::setWindowPos(center, NULL, { 0.5f, 0.5f });
					elements::setWindowSize({ 470.f, 235.f });
					elements::font(g_renderer->m_tahoma, [&] {
						elements::popupModal("Close?", [&] {
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
		g_manager.add(actionCommand("suicide", "Suicide", self::ped::suicide));
		//Self::Movement
		g_manager.add(toggleFloatCommand("run", "Run", self::movement::run));
		g_manager.add(toggleFloatCommand("swim", "Swim", self::movement::swim));
		g_manager.add(toggleFloatCommand("stamina", "Stamina", self::movement::stamina));
		g_manager.add(toggleFloatCommand("staminaRegeneration", "Stamina Regeneration", self::movement::staminaRegen));
		g_manager.add(toggleCommand("beastJump", "Beast Jump", self::movement::beastJump));
		g_manager.add(toggleCommand("superJump", "Super Jump", self::movement::superJump));
		g_manager.add(toggleCommand("gracefulLanding", "Graceful Landing", self::movement::gracefulLanding));
		g_manager.add(toggleFloatCommand("superRun", "Super Run", self::movement::superRun));
		g_manager.add(toggleFloatCommand("noClip", "No Clip", self::movement::noClip));
		//Self::Police
		g_manager.add(toggleCommand("neverWanted", "Never Wanted", "No police are spawned", self::police::neverWanted));
		g_manager.add(intCommand("wantedLevel", "Wanted Level", self::police::wantedLevel, true));
		//Self
		g_manager.add(intCommand("alpha", "Alpha", "Woah, I'm seethrough", self::alpha, true));
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
		//Network::Friends::Selected
		g_manager.add(actionCommand("copyFriendRid", "Copy RID", "Copies their RID to your clipboard", network::friends::selected::copyRid));
		g_manager.add(actionCommand("copyFriendName", "Copy Name", "Copies their name to your clipboard", network::friends::selected::copyName));
		//Network::Friends
		g_manager.add(variadicCommand("remove", "Remove friend", "Sends a SCAPI request to remove a friend", { { eValueType::String } }, network::friends::remove, false));
		//Network::Tunables
		g_manager.add(toggleCommand("offradar", "Off Radar", "Disappear like thin air", network::tunables::offRadar));
		//Network::Session::Browser
		g_manager.add(toggleIntCommand("matchmakingRegion", "Matchmaking Region", network::session::browser::matchmakingRegion));
		g_manager.add(toggleIntCommand("matchmakingLanguage", "Matchmaking Language", network::session::browser::matchmakingLanguage));
		g_manager.add(intCommand("matchmakingPlayerCountMinimum", "Matchmaking Player Count Minimum", network::session::browser::matchmakingPlayerCountMinimum));
		g_manager.add(intCommand("matchmakingPlayerCountMaximum", "Matchmaking Player Count Maximum", network::session::browser::matchmakingPlayerCountMaximum));
		g_manager.add(toggleIntCommand("matchmakingType", "Matchmaking Type", network::session::browser::matchmakingType));
		//Network::Session::Starter
		g_manager.add(variadicCommand("go", "Session Starter", "Join a session", { { eValueType::String } }, network::session::starter::go, false));
		g_manager.add(actionCommand("leave", "Leave", "Leave from online", network::session::starter::leave));
		//Network::Socialclub
		g_manager.add(variadicCommand("nameToRid", "Name To Rockstar ID", "Converts a given name to an RID and copies it to clipboard", { { eValueType::String } }, network::socialclub::nameToRid, false));
		g_manager.add(variadicCommand("ridToName", "Rockstar ID To Name", "Converts a given RID to an name and copies it to clipboard", { { eValueType::UInt64 } }, network::socialclub::ridToName, false));
		g_manager.add(variadicCommand("scMessage", "Message", "Messages them on Socialclub", { { eValueType::String }, { eValueType::String } }, network::socialclub::scMessage, false));
		g_manager.add(variadicCommand("convert", "Convert", "Converts a given name or RID to their counterpart and copies it to clipboard", { { eValueType::String } }, network::socialclub::convert, false));
		//Network
		g_manager.add(variadicCommand("join", "Join", "Join an player", { { eValueType::String } }, network::join, false));
		g_manager.add(actionCommand("bail", "Bail", "Bail from online", network::bail));
		//Protections::Kicks
		g_manager.add(protectionCommand("desyncKickProtection", "Desync", "May cause an modder detction"));
		g_manager.add(protectionCommand("lostConnectionKickProtection", "Lost Connection"));
		g_manager.add(protectionCommand("arrayOverrunKickProtection", "ScriptVM Overrun Kick", "This protects against all script event kicks that cause an array overflow"));
		//Protections::Crashes
		g_manager.add(protectionCommand("invalidRemoveWeaponCrashProtection", "Invalid weapon remove"));
		g_manager.add(protectionCommand("invalidObjectTypeCrashProtection", "Invalid object type"));
		g_manager.add(protectionCommand("mismatchObjectTypeCrashProtection", "Mismatch object type"));
		g_manager.add(protectionCommand("invalidSectorPositionCrashProtection", "Invalid sector position"));
		g_manager.add(protectionCommand("vehicleTempActionCrashProtection", "Vehicle Temp Action"));
		g_manager.add(protectionCommand("invalidTempActionCrashProtection", "Vehicle Temp Action"));
		g_manager.add(protectionCommand("invalidTrainTrackIndexCrashProtection", "Invalid train track index"));
		g_manager.add(protectionCommand("pedAttachCrashProtection", "Infinite ped attachment"));
		g_manager.add(protectionCommand("giveControlCrashProtection", "Invalid give control type"));
		g_manager.add(protectionCommand("invalidWordStateCrashProection", "Invalid world state"));
		//Protections::Events::Network
		g_manager.add(protectionCommand("freezeProtection", "Freeze"));
		g_manager.add(protectionCommand("ragdollProtection", "Ragdoll"));
		g_manager.add(protectionCommand("requestControlProtection", "Request Control"));
		g_manager.add(protectionCommand("giveWeaponProtection", "Give weapon"));
		g_manager.add(protectionCommand("removeWeaponProtection", "Remove weapon"));
		g_manager.add(protectionCommand("removeAllWeaponsProtection", "Remove all weapons"));
		g_manager.add(protectionCommand("voteKickProtection", "Vote kick"));
		//Protections
		g_manager.add(protectionCommand("vehicleKickProtection", "Vehicle Kick"));
		g_manager.add(protectionCommand("remoteTeleportProtecton", "Remote Teleport"));
		g_manager.add(protectionCommand("reportProtection", "Reports"));
		g_manager.add(protectionCommand("chatSpam", "Chat Spam", "Stops chat spam from being displayed"));
		//Settings::Ui
		g_manager.add(floatCommand("scale", "Scale", "Set the global UI scale", settings::ui::scale, true));
		//Settings::Game
		g_manager.add(toggleCommand("automp", "Auto Multiplayer", "Automatically load into multiplayer on startup", settings::game::autoMp));
		g_manager.add(toggleCommand("exitInstantly", "Exit Instantly", "Exits the game instantly when exit is requested", settings::game::exitInstantly));
		g_manager.add(actionCommand("unload", "Unload", "Removes " BRAND " from the game", settings::game::unload));
		g_manager.add(actionCommand("exit", "Exit", "Exit the game", settings::game::exit));
	}
	void onInit() {
		//These need to be after init because the values aren't created yet
		//Self::Movement
		"run"_TF->get(1).floating_point = 1.f;
		"swim"_TF->get(1).floating_point = 1.f;
		"stamina"_TF->get(1).floating_point = 11.f;
		"staminaRegeneration"_TF->get(1).floating_point = 1.f;
		"superRun"_TF->get(1).floating_point = 2.f;
		"noClip"_TF->get(1).floating_point = 1.f;
		//Self
		"alpha"_IF->get(0).i32 = 255;
		//Network::Session::Browser
		"matchmakingPlayerCountMinimum"_IF->get(0).i32 = 1;
		"matchmakingPlayerCountMaximum"_IF->get(0).i32 = 32;
		//Settings::Ui
		"scale"_FF->get(0).floating_point = 1.f;
	}
	void onTick() {
		Vector3 coords{ ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), !PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID())) };
		LOG(Info, "Coords: {}, {}, {}", coords.x, coords.y, coords.z);
		cPed = util::classes::getPed();
		cPedWeaponManager = cPed->m_weapon_manager;
		cWeaponInfo = cPedWeaponManager->m_weapon_info;
		cAmmoInfo = cWeaponInfo->m_ammo_info;
		cVehicle = cPed->m_vehicle;
		cPlayerInfo = util::classes::getPlayerInfo();
	}
}