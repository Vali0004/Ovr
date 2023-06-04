#pragma once
#include "pch/pch.h"
#include "renderer/renderer.h"
#include "hooking/hooking.h"
#include "commands/invoker/invoker.h"
#include "fiber/pool.h"
#include "script/script.h"
#include "features/features.h"
#include "exceptions/handler.h"

namespace core {
	namespace thread {
		extern void create(HMODULE hmodule);
		extern void loop();
		extern void destroy();
		extern DWORD entry(LPVOID paramater);
	}
	extern void create();
	extern void destroy();
}