#pragma once
#include "script/tabs/includes.h"

namespace tabs::protections {
	inline void crashes() {
		elements::tabItem("Crashes", [] {
			elements::protectionToggle("invalidRemoveWeaponCrashProtection", true);
			elements::protectionToggle("invalidObjectTypeCrashProtection");
			elements::protectionToggle("mismatchObjectTypeCrashProtection", true);
			elements::protectionToggle("invalidSectorPositionCrashProtection");
			elements::protectionToggle("vehicleTempActionCrashProtection", true);
			elements::protectionToggle("invalidTempActionCrashProtection");
			elements::protectionToggle("invalidTrainTrackIndexCrashProtection", true);
			elements::protectionToggle("pedAttachCrashProtection");
			elements::protectionToggle("giveControlCrashProtection", true);
			elements::protectionToggle("invalidWordStateCrashProection");
		});
	}
}