#pragma once
#include "pch/pch.h"
#include "notifications/notifications.h"

namespace script {
	inline float g_scale{ 1.f };
	inline bool g_guiOpen{};
	extern void onPresent();
	extern void onTick();
	extern void presentInit();
	extern void init();
}