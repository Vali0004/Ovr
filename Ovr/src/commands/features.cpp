#include "features.h"
#include "script/script.h"
#include "hooking/hooking.h"
#include <renderer/renderer.h>
#include <script/elements.h>

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
					cPed->m_armor = 255.f;
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
				/*if (command->get(0).toggle) {
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
				}*/
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
				Ped ped{ PLAYER::PLAYER_PED_ID() };
				if (command->get(0).toggle && !PED::IS_PED_IN_ANY_VEHICLE(ped, FALSE)) {
					ENTITY::SET_ENTITY_COLLISION(ped, FALSE, TRUE);
					Vector3 pos{ cPed->get_position().serialize() };
					Vector3 rot{ math::rotToDir(CAM::GET_GAMEPLAY_CAM_ROT(0)) };
					Vector3 camCoords{ pos + rot * command->get(1).floating_point };
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, pos, TRUE, TRUE, TRUE);
					if (PAD::IS_DISABLED_CONTROL_PRESSED(0, eControl::ControlSprint)) {
						camCoords = { pos + rot * command->get(1).floating_point * 3.f };
					}
					if (PAD::IS_DISABLED_CONTROL_PRESSED(0, eControl::ControlMoveUpOnly)) {
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, camCoords, TRUE, TRUE, TRUE);
					}
				}
				else {
					ONCE_PER_FRAME({ ENTITY::SET_ENTITY_COLLISION(ped, TRUE, TRUE); });
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
						pointers::g_handleToPointer(handle)->set_entity_flag(eEntityFlags::Visible, false);
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
				MISC::SET_TIME_SCALE(command->get(0).toggle ? 0.5f : 1.f);
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
			if (command->get(0).toggle) {
				cPed->m_entity_flags &= ~(uint32_t)eEntityFlags::Visible;
			}
			else {
				cPed->m_entity_flags |= (uint32_t)eEntityFlags::Visible;
			}
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
	}
	namespace vehicle {
		namespace spawner {
			void spawnVehicle(variadicCommand* command) {
				std::string key{ command->get(0).string };
				u32 hash{};
				if (isNumber(key)) {
					hash = stoi(key);
				}
				else if (containsANumber(key) && key.size() == 8) {
					hash = stoull(key);
				}
				else {
					hash = rage::joaat(key);
				}
				if (!STREAMING::IS_MODEL_VALID(hash)) {
					LOG(Commands, "{} is not a valid hash!", key);
				}
				else {
					while (!STREAMING::HAS_MODEL_LOADED(hash)) {
						STREAMING::REQUEST_MODEL(hash);
						fiber::current()->sleep();
					}
					Vector3 pos{ ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), TRUE) };
					float heading{ ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()) };
					Vehicle vehicle{ VEHICLE::CREATE_VEHICLE(hash, pos, heading, TRUE, TRUE, FALSE) };
					if (NETWORK::NETWORK_IS_SESSION_STARTED()) {
						DECORATOR::DECOR_SET_INT(vehicle, "MPBitset", 0);
					}
					PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), vehicle, -1);
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
				}
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
				nlohmann::json body = { { "RockstarId", rid } };
				auto res = socialclub::backend::jRequest(body, "https://scapi.rockstargames.com/friends/remove");
				LOG(Debug, "Body: {}", body.dump(4));
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
						printf("%s\n", response.c_str());
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
				void getGamerTask(u64 rid, std::function<void(rage::rlSessionByGamerTaskResult&)> onSuccess) {
					rage::rlGamerHandle handle{ rid };
					rage::rlSessionByGamerTaskResult result{};
					bool success{};
					rage::rlTaskStatus status{};
					if (pointers::g_getGamerTaskResult(0, &handle, 1, &result, 1, &success, &status)) {
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
				util::async([command] {
					u64 rid{ backend::nameToRid(command->get(0).string) };
					if (rid)
						g_engine.primitiveExecute("copyText {}", rid);
				});
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
				LOG(Commands, "Please provide a name or RID");
				return;
			}
			if (!isNumber(strRid)) {
				u64 rid{ socialclub::backend::nameToRid(strRid) };
				if (!rid) {
					LOG(Commands, "Failed to get {}'s RID", strRid);
					return;
				}
				strRid = std::to_string(rid);
			}
			u64 rid{ stoull(strRid) };
			g_fiberPool.add([rid] {
				if (HUD::GET_CURRENT_FRONTEND_MENU_VERSION() != 0xFFFFFFFF) {
					HUD::ACTIVATE_FRONTEND_MENU("FE_MENU_VERSION_SP_PAUSE"_joaat, false, 2);
					fiber::current()->sleep(200ms);
				}
				HUD::ACTIVATE_FRONTEND_MENU("FE_MENU_VERSION_SP_PAUSE"_joaat, false, 2);
				fiber::current()->sleep(200ms);
				CPlayerListMenu* ptr = new CPlayerListMenu();
				u32 Hash{ 0xDA4858C1 };
				FriendInfo& Friend{ util::network::friends::getFriends()[0] };
				u64 OriginalRID{ Friend.m_rockstar_id };
				u32 OriginialState{ Friend.m_friend_state };
				u32 WasJoinable{ Friend.m_is_joinable };
				Friend.m_rockstar_id = rid;
				Friend.m_friend_state = 3;
				Friend.m_is_joinable = 0x1;
				pointers::g_triggerPlayermenuAction(ptr, &Hash);
				fiber::current()->sleep(400ms);
				Friend.m_rockstar_id = OriginalRID;
				Friend.m_friend_state = OriginialState;
				Friend.m_is_joinable = WasJoinable;
			});
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
				"invalidSectorPositionCrashProtection"_PC->setFromSection(command->state());
				"vehicleTempActionCrashProtection"_PC->setFromSection(command->state());
				"invalidTempActionCrashProtection"_PC->setFromSection(command->state());
				"invalidTrainTrackIndexCrashProtection"_PC->setFromSection(command->state());
				"pedAttachCrashProtection"_PC->setFromSection(command->state());
				"giveControlCrashProtection"_PC->setFromSection(command->state());
				"invalidWordStateCrashProection"_PC->setFromSection(command->state());
				"scriptEventIslandHeliLaunchCrashProtection"_PC->setFromSection(command->state());
				"scriptEventSpawnVehicleCrashProtection"_PC->setFromSection(command->state());
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
				"networkBailProtection"_PC->setFromSection(command->state());
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
				"markPlayerBeastProtection"_PC->setFromSection(command->state());
				"sendTextLabelSMSProtection"_PC->setFromSection(command->state());
				"sendTextMessageProtection"_PC->setFromSection(command->state());
				"tseCommandProtection"_PC->setFromSection(command->state());
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
		}
	}
	namespace settings {
		namespace game {
			void unload(actionCommand* command) {
				g_running = false;
			}
			void exit(actionCommand* command) {
				g_renderer->m_callbacks.push_back(callback([](bool& active) {
					ONCE_PER_FRAME({ elements::openPopup("Close?"); });
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
		g_manager.add(toggleCommand("walkOnAir", "Walk On Air", self::movement::walkOnAir));
		//Self::World
		g_manager.add(toggleCommand("walkOnWater", "Walk On Water", "Walk on water, instead of swimming", self::world::walkOnWater));
		g_manager.add(toggleCommand("walkThroughWater", "Walk Through Water", "Allows you to walk right through water", self::world::walkThroughWater));
		g_manager.add(toggleCommand("slowMotion", "Slow Motion", "Time moves slower", self::world::slowMotion));
		//Self::Police
		g_manager.add(toggleCommand("neverWanted", "Never Wanted", "No police are spawned", self::police::neverWanted));
		g_manager.add(toggleCommand("lockWantedLevel", "Lock Wanted Level", "Stays at the same level when toggled", self::police::lockWantedLevel));
		g_manager.add(intCommand("wantedLevel", "Wanted Level", self::police::wantedLevel, true));
		g_manager.add(intCommand("fakeWantedLevel", "Fake Wanted Level", self::police::fakeWantedLevel, true));
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
		//Vehicle::Spawner
		g_manager.add(variadicCommand("spawnVehicle", "Spawn Vehicle", { { eValueType::String } }, &vehicle::spawner::spawnVehicle, false));
		//Network::Friends::Selected
		g_manager.add(actionCommand("copyFriendRid", "Copy RID", "Copies their RID to your clipboard", network::friends::selected::copyRid));
		g_manager.add(actionCommand("copyFriendName", "Copy Name", "Copies their name to your clipboard", network::friends::selected::copyName));
		//Network::Friends
		g_manager.add(variadicCommand("removeFriend", "Remove Friend", "Sends a SCAPI request to remove a friend", { { eValueType::String } }, network::friends::remove, false));
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
		g_manager.add(sectionProtectionCommand("allKickProtections", "All Kick Protections", "Sets all kick protections", protections::kicks::allKickProtections));
		g_manager.add(protectionCommand("desyncKickProtection", "Desync", "May cause an modder detction"));
		g_manager.add(protectionCommand("lostConnectionKickProtection", "Lost Connection"));
		g_manager.add(protectionCommand("arrayOverrunKickProtection", "ScriptVM Overrun Kick", "This protects against all script event kicks that cause an array overflow"));
		//Protections::Crashes
		g_manager.add(sectionProtectionCommand("allCrashProtections", "All Crash Protections", "Sets all crash protections", protections::crashes::allCrashProtections));
		g_manager.add(protectionCommand("invalidRemoveWeaponCrashProtection", "Invalid Weapon Remove"));
		g_manager.add(protectionCommand("invalidObjectTypeCrashProtection", "Invalid Object Type"));
		g_manager.add(protectionCommand("mismatchObjectTypeCrashProtection", "Mismatch Object Type"));
		g_manager.add(protectionCommand("invalidSectorPositionCrashProtection", "Invalid Sector Position"));
		g_manager.add(protectionCommand("vehicleTempActionCrashProtection", "Vehicle Temp Action"));
		g_manager.add(protectionCommand("invalidTempActionCrashProtection", "Invalid Temp Action"));
		g_manager.add(protectionCommand("invalidTrainTrackIndexCrashProtection", "Invalid Train Track Index"));
		g_manager.add(protectionCommand("pedAttachCrashProtection", "Infinite Ped Attachment"));
		g_manager.add(protectionCommand("giveControlCrashProtection", "Invalid Give Control Type"));
		g_manager.add(protectionCommand("invalidWordStateCrashProection", "Invalid World State"));
		g_manager.add(protectionCommand("scriptEventIslandHeliLaunchCrashProtection", "Script Event Island Heli Launch"));
		g_manager.add(protectionCommand("scriptEventSpawnVehicleCrashProtection", "Script Event Spawn Vehicle"));
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
		g_manager.add(protectionCommand("networkBailProtection", "Network Bail"));
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
		g_manager.add(protectionCommand("markPlayerBeastProtection", "Mark Player Beast"));
		g_manager.add(protectionCommand("kickFromInteriorProtection", "Kick From Interior"));
		g_manager.add(protectionCommand("interiorControlProtection", "Interior Control"));
		g_manager.add(protectionCommand("sendTextLabelSMSProtection", "Send Text Label SMS"));
		g_manager.add(protectionCommand("sendTextMessageProtection", "Send Text Message"));
		g_manager.add(protectionCommand("tseCommandProtection", "TSE Command"));
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
		g_manager.add(toggleCommand("automp", "Auto Multiplayer", "Automatically load into multiplayer on startup", miscellaneous::game::autoMp));
		g_manager.add(toggleCommand("exitInstantly", "Exit Instantly", "Exits the game instantly when exit is requested", miscellaneous::game::exitInstantly));
		//Settings::Ui
		g_manager.add(floatCommand("scale", "Scale", "Set the global UI scale", settings::ui::scale, true));
		//Settings::Game
		g_manager.add(actionCommand("unload", "Unload", "Removes " BRAND " from the game", settings::game::unload));
		g_manager.add(actionCommand("exit", "Exit", "Exit the game", settings::game::exit));
	}
	void uninit() {
		Ped ped{ PLAYER::PLAYER_PED_ID() };
		ENTITY::SET_ENTITY_COLLISION(ped, TRUE, TRUE);
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
		"run"_TC->get(1).floating_point = 1.f;
		"swim"_TC->get(1).floating_point = 1.f;
		"stamina"_TC->get(1).floating_point = 11.f;
		"staminaRegeneration"_TC->get(1).floating_point = 1.f;
		"superRun"_TC->get(1).floating_point = 2.f;
		"noClip"_TC->get(1).floating_point = 1.f;
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
		cPedWeaponManager = cPed->m_weapon_manager;
		cWeaponInfo = cPedWeaponManager->m_weapon_info;
		cAmmoInfo = cWeaponInfo->m_ammo_info;
		cVehicle = cPed->m_vehicle;
		cPlayerInfo = util::classes::getPlayerInfo();
	}
}