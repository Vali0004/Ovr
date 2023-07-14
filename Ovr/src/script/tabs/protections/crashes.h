#pragma once
#include "script/tabs/includes.h"

namespace tabs::protections {
	inline void crashes() {
		elements::menu("Crashes", [] {
			elements::sectionProtectionToggle("allCrashProtections");
			elements::protectionToggle("invalidRemoveWeaponCrashProtection");
			elements::protectionToggle("invalidObjectTypeCrashProtection");
			elements::protectionToggle("mismatchObjectTypeCrashProtection");
			elements::protectionToggle("invalidSectorPositionCrashProtection");
			elements::protectionToggle("vehicleTempActionCrashProtection");
			elements::protectionToggle("invalidTempActionCrashProtection");
			elements::protectionToggle("invalidTrainTrackIndexCrashProtection");
			elements::protectionToggle("pedAttachCrashProtection");
			elements::protectionToggle("giveControlCrashProtection");
			elements::protectionToggle("invalidWordStateCrashProection");
			elements::protectionToggle("scriptEventIslandHeliLaunchCrashProtection");
			elements::protectionToggle("scriptEventSyncedIntractionCrashProtection");
		});
	}
}