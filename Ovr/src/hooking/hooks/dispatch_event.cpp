#include "hooking/hooking.h"
#include "commands/manager/manager.h"
#include "util/util.h"

bool hooks::dispatchEvent(u64 _This, rage::netConMgr* pConMgr, rage::netConnection::InFrame* pEvent) {
	if (pEvent->GetId() == 0 || pEvent->GetId() == 4) {
		CNetGamePlayer* player{ util::network::getPlayerFromNetPacket(pEvent) };
		CNetGamePlayer* localPlayer{ util::network::getLocalNetGamePlayer() };
		rage::datBitBuffer buffer(pEvent->m_data, pEvent->m_length);
		buffer.m_flagBits = 1;
		if (eNetMessage type{}; player && util::network::deserialiseNetMessage(type, buffer)) {
			switch (type) {
			case eNetMessage::CMsgLostConnectionToHost: {
				uint64_t sessionId{};
				buffer.ReadQword(&sessionId, 64);
				rage::rlGamerHandle handle{ 0 };
				handle.deserialize(buffer);
				if (rage::rlGamerInfo* rlGamerInfo{ localPlayer->GetGamerInfo() }; rlGamerInfo && rlGamerInfo->m_unk_gamer_handle.m_rockstar_id == handle.m_rockstar_id) {
					switch ("lostConnectionKickProtection"_PF->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "H{} kick from {}", 0, player->GetName());
					} break;
					case eProtectionState::Block: {
						return true;
					} break;
					case eProtectionState::BlockAndNotify: {
						LOG(Session, "H{} kick from {}", 0, player->GetName());
						return true;
					} break;
					}
				}
			} break;
			case eNetMessage::CMsgNetComplaint: {
				if (!localPlayer->IsNetworkHost()) {
					if (CNetwork* network{ util::network::get() }) {
						int kickType{};
						CNetComplaintMgr& complaintMgr{ network->m_game_complaint_mgr };
						uint64_t peerAddress;
						buffer.ReadQword(&peerAddress, 64);
						u32 numberOfpeerAddresses{};
						buffer.ReadDword(&numberOfpeerAddresses, 32);
						if (numberOfpeerAddresses <= 64) {
							uint64_t arrayElement{};
							buffer.ReadQword(&arrayElement, 64);
							if (arrayElement != NULL) {
								switch ("desyncKickProtection"_PF->state()) {
								case eProtectionState::Notify: {
									LOG(Session, "D{} kick from {}", kickType, player->GetName());
								} break;
								case eProtectionState::Block: {
									complaintMgr.Remove(arrayElement);
									buffer.Seek(0);
									return false;
								} break;
								case eProtectionState::BlockAndNotify: {
									LOG(Session, "D{} from {}", kickType, player->GetName());
									complaintMgr.Remove(arrayElement);
									buffer.Seek(0);
									return false;
								} break;
								}
							}
						}
						if (complaintMgr.Count() < 1)
							kickType = 1;
						else
							kickType = 2;
						switch ("desyncKickProtection"_PF->state()) {
						case eProtectionState::Notify: {
							LOG(Session, "D{} from {}", kickType, player->GetName());
						} break;
						case eProtectionState::Block: {
							complaintMgr.RemoveAll();
							return false;
						} break;
						case eProtectionState::BlockAndNotify: {
							LOG(Session, "D{} from {}", kickType, player->GetName());
							complaintMgr.RemoveAll();
							return false;
						} break;
						}
					}
				}
			} break;
			case eNetMessage::CMsgKickPlayer: {
				uint64_t sessionId{};
				uint64_t peerId{};
				buffer.ReadQword(&sessionId, 64);
				buffer.ReadQword(&peerId, 64);
				eKickReason reason{ buffer.Read<eKickReason>(3) };
				buffer.Read<int>(32);
				switch (reason) {
				case eKickReason::VotedOut: {
					LOG(Session, "You were kicked by the host. Session rebound is not avaliable");
				} break;
				case eKickReason::PeerComplaints: {
					LOG(Session, "You got kicked by {}", player->GetName());
					pointers::g_joinBySessionInfo(util::network::get(), &util::network::get()->m_last_joined_session.m_session_info, 1, 1 | 2 | 4, nullptr, 0);
				} break;
				}
				return false;
			} break;
			case eNetMessage::CMsgTextMessage: {
				CMsgTextMessage msg{};
				buffer.ReadString(msg.m_message, sizeof(msg.m_message));
				buffer.ReadPeerId(&msg.m_peer_id);
				buffer.ReadBool(&msg.m_is_team);
				if (util::isSpamMessage(msg.m_message)) {
					switch ("chatSpam"_PF->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "{} is a chat spammer", player->GetName());
					} break;
					case eProtectionState::Block: {
						return true;
					} break;
					case eProtectionState::BlockAndNotify: {
						LOG(Session, "{} is a chat spammer", player->GetName());
						return true;
					} break;
					}
				}
				std::string label{ std::format("MP_CHAT_{}", msg.m_is_team ? "TEAM" : "ALL") };
				LOG_DIRECT(White, "Chat", "{} [{}]: {}", HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(label.c_str()), player->GetName(), msg.m_message);
			} break;
			case eNetMessage::CMsgTextMessage2: {
				CMsgTextMessage2 msg{};
				buffer.ReadString(msg.m_message, sizeof(msg.m_message));
				buffer.ReadPeerId(&msg.m_peer_id);
				if (util::isSpamMessage(msg.m_message)) {
					switch ("chatSpam"_PF->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "{} is a text message spammer", player->GetName());
					} break;
					case eProtectionState::Block: {
						return true;
					} break;
					case eProtectionState::BlockAndNotify: {
						LOG(Session, "{} is a text message spammer", player->GetName());
						return true;
					} break;
					}
				}
				LOG_DIRECT(White, "Text", "{}: {}", player->GetName(), msg.m_message);
			} break;
			}
		}
	}
	RET_CALL(dispatchEvent, _This, pConMgr, pEvent);
}