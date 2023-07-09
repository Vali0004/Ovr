#pragma once
#include "pch/pch.h"
#include "renderer/renderer.h"
#include "hooking/hooking.h"
#include "rage/commands/invoker/invoker.h"
#include "fiber/pool.h"
#include "script/script.h"
#include "commands/features.h"
#include "exceptions/handler.h"

namespace core {
	class scyllaHide : public shv::dynamicModule {
	public:
		scyllaHide() : shv::dynamicModule("Modules\\ScyllaHide.dll") {}
		~scyllaHide() {}
	};
	inline SmartPointer<scyllaHide> g_scyllaHide{};
	namespace thread {
		extern void create(HMODULE hmodule);
		extern void loop();
		extern void destroy();
		extern DWORD entry(LPVOID paramater);
	}
	extern void create();
	extern void destroy();
}