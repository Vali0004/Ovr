#pragma once
#include "script/tabs/includes.h"

namespace tabs::protections {
	inline void scriptEvents() {
		elements::menu("Script Events", [] {
			elements::sectionProtectionToggle("allScriptEventProtections");
			elements::protectionToggle("ceoBanProtection");
			elements::protectionToggle("ceoKickProtection");
			elements::protectionToggle("ceoMoneyProtection");
			elements::protectionToggle("clearWantedLevelProtection");
			elements::protectionToggle("forceMissionProtection");
			elements::protectionToggle("giveCollectableProtection");
			elements::protectionToggle("gtaBannerProtection");
			elements::protectionToggle("personalVehicleDestroyedProtection");
			elements::protectionToggle("remoteOffRadarProtection");
			elements::protectionToggle("sendToCutsceneProtection");
			elements::protectionToggle("sendToIslandProtection");
			elements::protectionToggle("sendToLocationProtection");
			elements::protectionToggle("soundSpamProtection");
			elements::protectionToggle("spectateProtection");
			elements::protectionToggle("teleportProtection");
			elements::protectionToggle("teleportToWarehouseProtection");
			elements::protectionToggle("vehicleKickProtection");
			elements::protectionToggle("mcTeleportProtection");
			elements::protectionToggle("startActivityProtection");
			elements::protectionToggle("kickFromInteriorProtection");
			elements::protectionToggle("sendTextLabelSMSProtection");
			elements::protectionToggle("sendTextMessageProtection");
			elements::protectionToggle("tseCommandRotateCamProtection");
			elements::protectionToggle("notificationProtection");
			elements::protectionToggle("customNotificationProtection");
			elements::protectionToggle("moneyBankedNotificationProtection");
			elements::protectionToggle("moneyStolenNotificationProtection");
			elements::protectionToggle("moneyRemovedNotificationProtection");
			elements::protectionToggle("destroyPersonalVehicleProtection");
			elements::protectionToggle("triggerCeoRaidProtection");
			elements::protectionToggle("startScriptBeginProtection");
			elements::protectionToggle("startScriptProceedProtection");
		});
	}
}