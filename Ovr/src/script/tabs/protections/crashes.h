#pragma once
#include "script/tabs/includes.h"

namespace tabs::protections {
	inline void crashes() {
		elements::menu("Crashes", [] {
			elements::sectionProtectionToggle("allCrashProtections");
			elements::protectionToggle("invalidRemoveWeaponCrashProtection");
			elements::protectionToggle("invalidObjectTypeCrashProtection");
			elements::protectionToggle("mismatchObjectTypeCrashProtection");
			elements::protectionToggle("invalidModelLODCrashProtection");
			elements::protectionToggle("invalidModelCrashProtection");
			elements::protectionToggle("invalidModelVehicleCrashProtection");
			elements::protectionToggle("invalidModelPedCrashProtection");
			elements::protectionToggle("invalidSectorPositionCrashProtection");
			elements::protectionToggle("vehicleTempActionCrashProtection");
			elements::protectionToggle("invalidTempActionCrashProtection");
			elements::protectionToggle("invalidTrainTrackIndexCrashProtection");
			elements::protectionToggle("pedAttachCrashProtection");
			elements::protectionToggle("giveControlCrashProtection");
			elements::protectionToggle("ropeCrashProtection");
			elements::protectionToggle("invalidWordStateCrashProection");
			elements::protectionToggle("pedMountFlagCrashProtection");
			elements::protectionToggle("pedStateGadgetOverflowCrashProtection");
			elements::protectionToggle("invalidGadgetCrashProtection");
			elements::protectionToggle("submarineCarCrashProtection");
			elements::protectionToggle("scriptEventIslandHeliLaunchCrashProtection");
			elements::protectionToggle("scriptEventRequestVehicleSpawnCrashProtection");
			elements::protectionToggle("scriptEventSyncedIntractionCrashProtection");
		});
	}
}