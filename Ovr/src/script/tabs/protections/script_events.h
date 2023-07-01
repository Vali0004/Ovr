#pragma once
#include "script/tabs/includes.h"

namespace tabs::protections {
	inline void scriptEvents() {
		elements::tabItem("Script Events", [] {
			elements::sectionProtectionToggle("allScriptEventProtections");
			elements::protectionToggle("ceoBanProtection", true);
			elements::protectionToggle("ceoKickProtection", true);
			elements::protectionToggle("ceoMoneyProtection");
			elements::protectionToggle("clearWantedLevelProtection", true);
			elements::protectionToggle("forceMissionProtection", true);
			elements::protectionToggle("giveCollectableProtection");
			elements::protectionToggle("gtaBannerProtection", true);
			elements::protectionToggle("networkBailProtection", true);
			elements::protectionToggle("personalVehicleDestroyedProtection");
			elements::protectionToggle("remoteOffRadarProtection", true);
			elements::protectionToggle("sendToCutsceneProtection", true);
			elements::protectionToggle("sendToIslandProtection");
			elements::protectionToggle("sendToLocationProtection", true);
			elements::protectionToggle("soundSpamProtection", true);
			elements::protectionToggle("spectateProtection");
			elements::protectionToggle("teleportProtection", true);
			elements::protectionToggle("teleportToWarehouseProtection", true);
			elements::protectionToggle("vehicleKickProtection");
			elements::protectionToggle("mcTeleportProtection", true);
			elements::protectionToggle("startActivityProtection", true);
			elements::protectionToggle("markPlayerBeastProtection");
			elements::protectionToggle("sendTextLabelSMSProtection", true);
			elements::protectionToggle("sendTextMessageProtection", true);
			elements::protectionToggle("tseCommandProtection");
			elements::protectionToggle("tseCommandRotateCamProtection", true);
			elements::protectionToggle("notificationProtection", true);
			elements::protectionToggle("customNotificationProtection");
			elements::protectionToggle("moneyBankedNotificationProtection", true);
			elements::protectionToggle("moneyStolenNotificationProtection", true);
			elements::protectionToggle("moneyRemovedNotificationProtection");
			elements::protectionToggle("destoryPersonalVehicleProtection", true);
			elements::protectionToggle("triggerCeoRaidProtection", true);
			elements::protectionToggle("startScriptBeginProtection");
			elements::protectionToggle("startScriptProceedProtection", true);
		});
	}
}