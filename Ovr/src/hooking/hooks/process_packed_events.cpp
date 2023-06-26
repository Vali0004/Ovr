#include "hooking/hooking.h"
#include "commands/manager/manager.h"
#include "util/util.h"
#define PROT_CHECK(c) switch (c##_PC->state()) { \
	case eProtectionState::Notify: { \
		LOG(Session, "{} from {}", c##_PC->m_name, Sender->GetName()); \
	} break; \
	case eProtectionState::Block: { \
		return true; \
	} break; \
	case eProtectionState::BlockAndNotify: { \
		LOG(Session, "{} from {}", c##_PC->m_name, Sender->GetName()); \
		return true; \
	} break; \
}
#define EVNT_PROT_CHECK(c) switch (c##_PC->state()) { \
	case eProtectionState::Notify: { \
		LOG(Session, "{} from {}", c##_PC->m_name, Sender->GetName()); \
	} break; \
	case eProtectionState::Block: { \
		g_statistics.m_incomingNetworkEvents--; \
		return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset); \
	} break; \
	case eProtectionState::BlockAndNotify: { \
		LOG(Session, "{} from {}", c##_PC->m_name, Sender->GetName()); \
		g_statistics.m_incomingNetworkEvents--; \
		return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset); \
	} break; \
}

bool scriptedGameEvent(CScriptedGameEvent* pEvent, CNetGamePlayer* Sender) {
	g_statistics.m_lastScriptEventSender.update(Sender);
	#define CASE(e, n) case eScriptEvents::##e: { PROT_CHECK(n); } break
	switch (static_cast<eScriptEvents>(pEvent->m_args[0])) {
	CASE(ScriptEventGbDisableGoonMembership, "ceoBanProtection");
	CASE(ScriptEventGbBossStartedUsingSctv, "ceoKickProtection");
	CASE(ScriptEventBeenPaidGoonCash, "ceoMoneyProtection");
	CASE(ScriptEventRemoveWantedLevel, "clearWantedLevelProtection");
	CASE(ScriptEventBossShouldLaunchWvm, "forceMissionProtection");
	CASE(ScriptEventCollectibleCollected, "giveCollectableProtection");
	CASE(ScriptEventFmDeliverableDelivery, "gtaBannerProtection");
	CASE(ScriptEventBailMeForSctv, "networkBailProtection");
	CASE(ScriptEventCarInsurance, "personalVehicleDestroyedProtection");
	CASE(ScriptEventOffTheRadar, "remoteOffRadarProtection");
	CASE(ScriptEventCasinoPlayHeistCutscene, "sendToCutsceneProtection");
	CASE(ScriptEventInviteToHeistIslandBeachParty, "sendToIslandProtection");
	CASE(ScriptEventHeistIslandWarpTransition, "sendToLocationProtection");
	CASE(ScriptEventInviteToApartment, "soundSpamProtection");
	CASE(TickerEventSpectatorStart, "spectateProtection");
	CASE(ScriptEventInviteNearbyPlayersIntoApartment, "teleportProtection");
	CASE(ScriptEventWarpToQuickTravelDestination, "teleportToWarehouseProtection");
	CASE(ScriptEventLeaveVehicle, "gentleVehicleKickProtection");
	CASE(ScriptEventGroupWarp, "mcTeleportProtection");
	CASE(ScriptEventGbNonBossChallengeRequest, "startActivityProtection");
	CASE(ScriptEventHuntTheBeastUpdateGlobal, "markPlayerBeastProtection");
	CASE(ScriptEventTriggerExitAllFromSimpleInterior, "kickFromInteriorProtection");
	CASE(ScriptEventPlayersWarpInsideSimpleInterior, "interiorControlProtection");
	CASE(ScriptEventSendBasicText, "sendTextLabelSMSProtection");
	CASE(ScriptEventTextMessageUsingLiteral, "sendTextMessageProtection");
	CASE(ScriptEventGeneral, "tseCommandProtection");
	CASE(GeneralEventHeistPreplanExitGuestMode, "tseCommandRotateCamProtection");
	CASE(ScriptEventTickerMessage, "notificationProtection");
	CASE(ScriptEventTickerMessageCustom, "customNotificationProtection");
	CASE(TickerEventTeamCashBanked, "moneyBankedNotificationProtection");
	CASE(TickerEventTeamCashStolen, "moneyStolenNotificationProtection");
	CASE(TickerEventTeamCashRemoved, "moneyRemovedNotificationProtection");
	CASE(ScriptEventPersonalVehicleStolen, "destoryPersonalVehicleProtection");
	CASE(ScriptEventGbTriggerDefendMission, "triggerCeoRaidProtection");
	CASE(ScriptEventConfirmationLaunchMission, "startScriptBeginProtection");
	CASE(ScriptEventForcePlayerOntoMission, "startScriptProceedProtection");
	case eScriptEvents::ScriptEventIslandBackupHeliLaunch: {
		switch ("scriptEventIslandHeliLaunchCrashProtection"_PC->state()) {
		case eProtectionState::Notify: {
			LOG(Session, "S{} from {}", 0, Sender->GetName());
		} break;
		case eProtectionState::Block: {
			return true;
		} break;
		case eProtectionState::BlockAndNotify: {
			LOG(Session, "S{} from {}", 0, Sender->GetName());
			return true;
		} break;
		}
	} break;
	case eScriptEvents::ScriptEventRequestToSpawnVehicle: {
		switch ("scriptEventSyncedIntractionCrashProtection"_PC->state()) {
		case eProtectionState::Notify: {
			LOG(Session, "S{} from {}", 1, Sender->GetName());
		} break;
		case eProtectionState::Block: {
			return true;
		} break;
		case eProtectionState::BlockAndNotify: {
			LOG(Session, "S{} from {}", 1, Sender->GetName());
			return true;
		} break;
		}
	} break;
	case eScriptEvents::ScriptEventLaunchSyncedInteraction: {
		switch ("scriptEventSyncedIntractionCrashProtection"_PC->state()) {
		case eProtectionState::Notify: {
			LOG(Session, "S{} from {}", 2, Sender->GetName());
		} break;
		case eProtectionState::Block: {
			return true;
		} break;
		case eProtectionState::BlockAndNotify: {
			LOG(Session, "S{} from {}", 2, Sender->GetName());
			return true;
		} break;
		}
	} break;
	}
	return false;
}
bool incrementStatEvent(CNetworkIncrementStatEvent* pEvent, CNetGamePlayer* Sender) {
	switch (pEvent->m_stat) {
	case "mpply_bad_crew_status"_joaat:
	case "mpply_bad_crew_motto"_joaat:
	case "mpply_bad_crew_name"_joaat:
	case "mpply_bad_crew_emblem"_joaat:
	case "mpply_exploits"_joaat:
	case "mpply_game_exploits"_joaat:
	case "mpply_tc_annoyingme"_joaat:
	case "mpply_tc_hate"_joaat:
	case "mpply_vc_annoyingme"_joaat:
	case "mpply_vc_hate"_joaat: {
		PROT_CHECK("reportProtection");
	} break;
	}
	return false;
}
void hooks::proccessPackedEvents(rage::netEventMgr* pEventMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, u16 Id, i32 Index, i32 HandledBitset, i32 BufferSize, rage::datBitBuffer* Buffer) {
	if (Id > 91u)
		return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
	ccp eventName{ *(ccp*)(u64(pEventMgr) + 8ui64 * Id + 0x3B6B0) };
	if (eventName == nullptr || Sender == nullptr || Sender->m_player_id < 0 || Sender->m_player_id >= 32)
		return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
	g_statistics.m_incomingNetworkEvents++;
	switch (static_cast<ePackedEvents>(Id)) {
	case ePackedEvents::CScriptedGameEvent: {
		std::unique_ptr<CScriptedGameEvent> event{ std::make_unique<CScriptedGameEvent>() };
		Buffer->ReadDword(&event->m_args_size, 32);
		if (event->m_args_size - 1 <= 0x1AF) {
			Buffer->ReadArray(&event->m_args, 8 * event->m_args_size);
		}
		if (scriptedGameEvent(event.get(), Sender)) {
			g_statistics.m_incomingNetworkEvents--;
			return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
		}
		Buffer->Seek(0);
	} break;
	case ePackedEvents::CNetworkIncrementStatEvent: {
		std::unique_ptr<CNetworkIncrementStatEvent> event{ std::make_unique<CNetworkIncrementStatEvent>() };
		Buffer->ReadDword(&event->m_stat, 32);
		Buffer->ReadDword(&event->m_amount, 32);
		if (incrementStatEvent(event.get(), Sender)) {
			g_statistics.m_incomingNetworkEvents--;
			return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
		}
		Buffer->Seek(0);
	} break;
	case ePackedEvents::CScriptEntityStateChangeEvent: {
		u16 entity{ Buffer->Read<u16>(13) };
		eScriptEntityChangeType type{ Buffer->Read<eScriptEntityChangeType>(4) };
		u32 unknown{ Buffer->Read<u32>(32) };
		switch (type) {
		case eScriptEntityChangeType::SettingOfTaskVehicleTempAction: {
			u16 ped{ Buffer->Read<u16>(13) };
			u16 action{ Buffer->Read<u16>(8) };
			if ((action >= 15 && action <= 18) || action == 33) {
				switch ("vehicleTempActionCrashProtection"_PC->state()) {
				case eProtectionState::Notify: {
					LOG(Session, "V{} from {}", 1, Sender->GetName());
				} break;
				case eProtectionState::Block: {
					g_statistics.m_incomingNetworkEvents--;
					return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
				} break;
				case eProtectionState::BlockAndNotify: {
					LOG(Session, "V{} from {}", 1, Sender->GetName());
					g_statistics.m_incomingNetworkEvents--;
					return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
				} break;
				}
			}
		} break;
		}
		if (static_cast<u32>(type) > 9 || static_cast<u32>(type) < 0) {
			switch ("invalidTempActionCrashProtection"_PC->state()) {
			case eProtectionState::Notify: {
				LOG(Session, "T{} crash from {}", 5, Sender->GetName());
			} break;
			case eProtectionState::Block: {
				g_statistics.m_incomingNetworkEvents--;
				return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
			} break;
			case eProtectionState::BlockAndNotify: {
				LOG(Session, "T{} crash from {}", 5, Sender->GetName());
				g_statistics.m_incomingNetworkEvents--;
				return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
			} break;
			}
		}
		Buffer->Seek(0);
	} break;
	case ePackedEvents::CGiveControlEvent: {
		u32 timestamp{ Buffer->Read<u32>(32) };
		i32 count{ Buffer->Read<i32>(2) };
		bool allObjectsMigrateTogether{ Buffer->Read<bool>(1) };
		if (count > 3) {
			count = 3;
		}
		for (i32 i{}; i != count; ++i) {
			i32 networkId{ Buffer->Read<i32>(13) };
			eNetObjectType objectType{ Buffer->Read<eNetObjectType>(4) };
			i32 migrationType{ Buffer->Read<i32>(3) };
			if (objectType < eNetObjectType::Automobile || objectType > eNetObjectType::Train) {
				switch ("giveControlCrashProtection"_PC->state()) {
				case eProtectionState::Notify: {
					LOG(Session, "T{} crash from {}", 6, Sender->GetName());
				} break;
				case eProtectionState::Block: {
					g_statistics.m_incomingNetworkEvents--;
					return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
				} break;
				case eProtectionState::BlockAndNotify: {
					LOG(Session, "T{} crash from {}", 6, Sender->GetName());
					g_statistics.m_incomingNetworkEvents--;
					return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
				} break;
				}
			}
		}
		Buffer->Seek(0);
	} break;
	case ePackedEvents::CNetworkClearPedTasksEvent: {
		i32 networkId{ Buffer->Read<i32>(13) };
		if (CPed* ped{ util::classes::getPed() }) {
			if (rage::netObject* netObject{ ped->m_net_object }) {
				if (netObject->m_object_id == networkId) {
					EVNT_PROT_CHECK("freezeProtection");
				}
			}
		}
		Buffer->Seek(0);
	} break;
	case ePackedEvents::CRagdollRequestEvent: {
		i32 networkId{ Buffer->Read<i32>(13) };
		if (CPed* ped{ util::classes::getPed() }) {
			if (rage::netObject* netObject{ ped->m_net_object }) {
				if (netObject->m_object_id == networkId) {
					EVNT_PROT_CHECK("ragdollProtection");
				}
			}
		}
		Buffer->Seek(0);
	} break;
	case ePackedEvents::CRequestControlEvent: {
		i32 networkId{ Buffer->Read<i32>(13) };
		if (CPed* ped{ util::classes::getPed() }) {
			if (CVehicle* vehicle{ ped->m_vehicle }) {
				if (rage::netObject* netObject{ vehicle->m_net_object }) {
					if (netObject->m_object_id == networkId && vehicle->m_driver == ped) {
						EVNT_PROT_CHECK("requestControlProtection");
					}
				}
			}
		}
		Buffer->Seek(0);
	} break;
	case ePackedEvents::CScriptWorldStateEvent: {
		eWorldStateDataType Type{ Buffer->Read<eWorldStateDataType>(4) };
		bool Unknown{ Buffer->Read<bool>(1) };
		CGameScriptId id{};
		id.deserialize(*Buffer);
		switch (Type) {
		case eWorldStateDataType::Rope: {
			i32 networkRopeId{ Buffer->Read<i32>(9) };
			Vector3 pos{};
			pos.x = Buffer->Read<float>(19);
			pos.y = Buffer->Read<float>(19);
			pos.z = Buffer->Read<float>(19);
			Vector3 rot{};
			rot.x = Buffer->Read<float>(19);
			rot.y = Buffer->Read<float>(19);
			rot.z = Buffer->Read<float>(19);
			float length{ Buffer->Read<float>(16) };
			i32 type{ Buffer->Read<i32>(4) };
			float initialLength{ Buffer->Read<float>(16) };
			float minLength{ Buffer->Read<float>(16) };
			if (type == 0 || initialLength < minLength) {
				switch ("ropeCrashProtection"_PC->state()) {
				case eProtectionState::Notify: {
					LOG(Session, "R{} crash from {}", 0, Sender->GetName());
				} break;
				case eProtectionState::Block: {
					g_statistics.m_incomingNetworkEvents--;
					return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
				} break;
				case eProtectionState::BlockAndNotify: {
					LOG(Session, "R{} crash from {}", 0, Sender->GetName());
					g_statistics.m_incomingNetworkEvents--;
					return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
				} break;
				}
			}
		} break;
		case eWorldStateDataType::PopGroupOverride: {
			i32 popSchedule{ Buffer->ReadSigned<i32>(8) };
			i32 popGroup{ Buffer->Read<i32>(32) };
			i32 percentage{ Buffer->Read<i32>(7) };
			if (popGroup == 0) {
				if (percentage == 0 || percentage == 103) {
					switch ("invalidPopGroupCrashProtection"_PC->state()) {
					case eProtectionState::Notify: {
						LOG(Session, "P{} crash from {}", 3, Sender->GetName());
					} break;
					case eProtectionState::Block: {
						g_statistics.m_incomingNetworkEvents--;
						return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
					} break;
					case eProtectionState::BlockAndNotify: {
						LOG(Session, "P{} crash from {}", 3, Sender->GetName());
						g_statistics.m_incomingNetworkEvents--;
						return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
					} break;
					}
				}
			}
		} break;
		}
		if (static_cast<u32>(Type) > 9 || static_cast<u32>(Type) < 0) {
			switch ("invalidWordStateCrashProection"_PC->state()) {
			case eProtectionState::Notify: {
				LOG(Session, "T{} crash from {}", 7, Sender->GetName());
			} break;
			case eProtectionState::Block: {
				g_statistics.m_incomingNetworkEvents--;
				return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
			} break;
			case eProtectionState::BlockAndNotify: {
				LOG(Session, "T{} crash from {}", 7, Sender->GetName());
				g_statistics.m_incomingNetworkEvents--;
				return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
			} break;
			}
		}
		Buffer->Seek(0);
	} break;
	case ePackedEvents::CRemoveWeaponEvent: {
		i32 networkId{ Buffer->Read<i32>(13) };
		u32 Hash{ Buffer->Read<u32>(32) };
		if (Hash == "weapon_unarmed"_joaat) {
			switch ("invalidRemoveWeaponCrashProtection"_PC->state()) {
			case eProtectionState::Notify: {
				LOG(Session, "R{} crash from {}", 1, Sender->GetName());
			} break;
			case eProtectionState::Block: {
				g_statistics.m_incomingNetworkEvents--;
				return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
			} break;
			case eProtectionState::BlockAndNotify: {
				LOG(Session, "R{} crash from {}", 1, Sender->GetName());
				g_statistics.m_incomingNetworkEvents--;
				return pointers::g_sendEventAck(pEventMgr, Sender, Receiver, Index, HandledBitset);
			} break;
			}
		}
		if (CPed* ped{ util::classes::getPed() }) {
			if (rage::netObject* netObject{ ped->m_net_object }) {
				if (netObject->m_object_id == networkId) {
					EVNT_PROT_CHECK("removeWeaponProtection");
				}
			}
		}
		Buffer->Seek(0);
	} break;
	case ePackedEvents::CGiveWeaponEvent: {
		i32 networkId{ Buffer->Read<i32>(13) };
		if (CPed* ped{ util::classes::getPed() }) {
			if (rage::netObject* netObject{ ped->m_net_object }) {
				if (netObject->m_object_id == networkId) {
					EVNT_PROT_CHECK("giveWeaponProtection");
				}
			}
		}
		Buffer->Seek(0);
	} break;
	case ePackedEvents::CRemoveAllWeaponsEvent: {
		i32 networkId{ Buffer->Read<i32>(13) };
		if (CPed* ped{ util::classes::getPed() }) {
			if (rage::netObject* netObject{ ped->m_net_object }) {
				if (netObject->m_object_id == networkId) {
					EVNT_PROT_CHECK("removeAllWeaponsProtection");
				}
			}
		}
		Buffer->Seek(0);
	} break;
	case ePackedEvents::CKickVotesEvent: {
		uint32_t bitset{};
		Buffer->ReadDword(&bitset, 0x20);
		if (bitset & (1 << Receiver->m_player_id)) {
			EVNT_PROT_CHECK("voteKickProtection");
		}
		Buffer->Seek(0);
	} break;
	}
	g_statistics.m_incomingNetworkEvents--;
	RET_CALL(proccessPackedEvents, pEventMgr, Sender, Receiver, Id, Index, HandledBitset, BufferSize, Buffer);
}