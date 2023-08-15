#pragma once
#include "script/tabs/includes.h"
#include "commands/engine/engine.h"

namespace tabs::settings {
	inline void statistics() {
		elements::menu("Statistics", [] {
			ccheckbox("drawNativesInvokedByGame"_TC);
			ccheckbox("drawNativesInvokedBy" BRAND ""_TC);
			ccheckbox("drawPoolCounts"_TC);
			ccheckbox("drawCoordinates"_TC);
			ccheckbox("drawHeading"_TC);
			ccheckbox("drawPlayerCount"_TC);
			ccheckbox("drawIncomingNetworkEventCount"_TC);
			ccheckbox("drawFrameData"_TC);
			ccheckbox("drasFramesPerSecond"_TC);
			ccheckbox("drawSessionType"_TC);
			ccheckbox("drawHostName"_TC);
			ccheckbox("drawNextHostName"_TC);
			ccheckbox("drawScriptHostName"_TC);
			ccheckbox("drawLastScriptEventSenderName"_TC);
			ccheckbox("drawLocalPlayerName"_TC);
		});
	}
}