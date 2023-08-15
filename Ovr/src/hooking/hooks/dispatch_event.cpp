#include "hooking/hooking.h"
#include "commands/manager/manager.h"
#include "util/util.h"

bool hooks::dispatchEvent(u64 _This, rage::netConnectionManager* pConMgr, rage::netConnection::InFrame* pEvent) {
	if (pEvent->GetId() == 0 || pEvent->GetId() == 4) {
		util::network::player player{ util::network::g_manager.getBySnPlayerPlatformData(pEvent->m_peer_id) };
		util::network::player localPlayer{ util::network::g_manager.local() };
		rage::datBitBuffer buffer(pEvent->m_data, pEvent->m_length);
		buffer.m_flagBits = 1;
		if (eNetMessage type{}; player && util::network::deserialiseNetMessage(type, buffer)) {
			g_statistics.m_incomingNetworkEvents++;
			switch (type) {
			case eNetMessage::CMsgLostConnectionToHost: {
				uint64_t sessionId{};
				buffer.ReadQword(&sessionId, 64);
				rage::rlGamerHandle handle{ 0 };
				handle.deserialize(buffer);
				if (player.m_rid == handle.m_rockstar_id) {
					switch ("lostConnectionKickProtection"_PC->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "H{} kick from {}", 0, player.m_name);
					} break;
					case eProtectionState::Block: {
						return true;
					} break;
					case eProtectionState::BlockAndNotify: {
						LOG(Session, "H{} kick from {}", 0,  player.m_name);
						return true;
					} break;
					}
				}
			} break;
			case eNetMessage::CMsgNetComplaint: {
				if (!localPlayer.m_host) {
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
								switch ("desyncKickProtection"_PC->state()) {
								case eProtectionState::Notify: {
									LOG(Session, "D{} kick from {}", kickType,  player.m_name);
								} break;
								case eProtectionState::Block: {
									complaintMgr.Remove(arrayElement);
									buffer.Seek(0);
									return false;
								} break;
								case eProtectionState::BlockAndNotify: {
									LOG(Session, "D{} from {}", kickType,  player.m_name);
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
						switch ("desyncKickProtection"_PC->state()) {
						case eProtectionState::Notify: {
							LOG(Session, "D{} from {}", kickType,  player.m_name);
						} break;
						case eProtectionState::Block: {
							complaintMgr.RemoveAll();
							return false;
						} break;
						case eProtectionState::BlockAndNotify: {
							LOG(Session, "D{} from {}", kickType,  player.m_name);
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
					LOG(Session, "You were kicked by {} who was not a host. Session rebound is avaliable",  player.m_name);
					pointers::g_joinBySessionInfo(util::network::get(), &util::network::get()->m_last_joined_session.m_session_info, 1, 1 | 2 | 4, nullptr, 0);
				} break;
				case eKickReason::ConnectionError: {
					LOG(Session, "Connection error. Session rebound is avaliable",  player.m_name);
					pointers::g_joinBySessionInfo(util::network::get(), &util::network::get()->m_last_joined_session.m_session_info, 1, 1 | 2 | 4, nullptr, 0);
				} break;
				case eKickReason::NatType: {
					LOG(Session, "Connection error. Session rebound is avaliable",  player.m_name);
					pointers::g_joinBySessionInfo(util::network::get(), &util::network::get()->m_last_joined_session.m_session_info, 1, 1 | 2 | 4, nullptr, 0);
				} break;
				}
				return false;
			} break;
			case eNetMessage::CMsgPackedReliables: {
				CMsgPackedReliables msg{};
				if (msg.Deserialise(&buffer)) {
					auto messageBuffer = &msg.m_creates.m_buffer;
					while (messageBuffer->GetDataLength() - messageBuffer->GetPosition() >= 38) {
						uint16_t objectType{}, objectId{}, flags{};
						uint32_t bits{};
						uint8_t createData[4096]{};
						messageBuffer->ReadWord(&objectType, 4);
						messageBuffer->ReadWord(&objectId, 13);
						messageBuffer->ReadWord(&flags, 8);
						messageBuffer->ReadDword(&bits, 13);
						if (bits > 0 && messageBuffer->ReadArray(createData, bits)) {
							rage::datBitBuffer createBuffer(createData, bits, false);
							createBuffer.m_maxBit = bits + 1;
							createBuffer.Seek(0);
							util::network::getObjectMgr()->HandleCloneCreate(player.m_netGamePlayer, util::network::g_manager.local().m_netGamePlayer, objectType, objectId, flags, &createBuffer, 0);
						}
					}
					messageBuffer = &msg.m_create_acks.m_buffer;
					while (messageBuffer->GetDataLength() - messageBuffer->GetPosition() >= 38) {
						uint16_t objectId{};
						uint32_t ackCode{};
						messageBuffer->ReadWord(&objectId, 13u);
						messageBuffer->ReadDword(&ackCode, 4u);
						util::network::getObjectMgr()->HandleCloneCreateAck(player.m_netGamePlayer, util::network::g_manager.local().m_netGamePlayer, objectId, ackCode);
					}
					messageBuffer = &msg.m_removes.m_buffer;
					while (messageBuffer->GetMaxDataLength() - messageBuffer->GetPosition() >= 18) {
						uint16_t objectId{};
						uint32_t token{};
						messageBuffer->ReadWord(&objectId, 13u);
						messageBuffer->ReadDword(&token, 5u);
						util::network::getObjectMgr()->HandleCloneRemove(player.m_netGamePlayer, util::network::g_manager.local().m_netGamePlayer, objectId, token);
					}
					messageBuffer = &msg.m_remove_acks.m_buffer;
					while (messageBuffer->GetDataLength() - messageBuffer->GetPosition() >= 38) {
						uint16_t objectId{};
						uint32_t ackCode{};
						messageBuffer->ReadWord(&objectId, 13u);
						messageBuffer->ReadDword(&ackCode, 4u);
						util::network::getObjectMgr()->HandleCloneRemoveAck(player.m_netGamePlayer, util::network::g_manager.local().m_netGamePlayer, objectId, ackCode);
					}
					return false;
				}
			} break;
			case eNetMessage::CMsgTextMessage: {
				CMsgTextMessage msg{};
				buffer.ReadString(msg.m_message, sizeof(msg.m_message));
				buffer.ReadPeerId(&msg.m_peer_id);
				buffer.ReadBool(&msg.m_is_team);
				if (util::isSpamMessage(msg.m_message)) {
					switch ("chatSpamProtection"_PC->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "{} is a chat spammer",  player.m_name);
					} break;
					case eProtectionState::Block: {
						return true;
					} break;
					case eProtectionState::BlockAndNotify: {
						LOG(Session, "{} is a chat spammer",  player.m_name);
						return true;
					} break;
					}
				}
				std::string label{ std::format("MP_CHAT_{}", msg.m_is_team ? "TEAM" : "ALL") };
				LOG_DIRECT(White, "Chat", "{} [{}]: {}", HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(label.c_str()), player.m_name, msg.m_message);
			} break;
			case eNetMessage::CMsgTextMessage2: {
				CMsgTextMessage2 msg{};
				buffer.ReadString(msg.m_message, sizeof(msg.m_message));
				buffer.ReadPeerId(&msg.m_peer_id);
				if (util::isSpamMessage(msg.m_message)) {
					switch ("chatSpamProtection"_PC->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "{} is a text message spammer",  player.m_name);
					} break;
					case eProtectionState::Block: {
						return true;
					} break;
					case eProtectionState::BlockAndNotify: {
						LOG(Session, "{} is a text message spammer",  player.m_name);
						return true;
					} break;
					}
				}
				LOG_DIRECT(White, "Text", "{}: {}", player.m_name, msg.m_message);
			} break;
			}
		}
	}
	RET_CALL(dispatchEvent, _This, pConMgr, pEvent);
}