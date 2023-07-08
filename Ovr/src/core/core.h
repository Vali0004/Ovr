#pragma once
#include "pch/pch.h"
#include "renderer/renderer.h"
#include "hooking/hooking.h"
#include "rage/commands/invoker/invoker.h"
#include "fiber/pool.h"
#include "script/script.h"
#include "commands/features.h"
#include "exceptions/handler.h"

namespace core {/*
	class scyllaHide : public shv::dynamicLoader {
	public:
		scyllaHide() : shv::dynamicLoader(m_path.append(std::getenv("appdata")).append(BRAND"\\Modules").append("ScyllaHide.dll")) {}
		~scyllaHide() {}
	private:
		fs::path m_path{};
	};
	inline std::unique_ptr<scyllaHide> g_scyllaHide{};*/
	namespace thread {
		extern void create(HMODULE hmodule);
		extern void loop();
		extern void destroy();
		extern DWORD entry(LPVOID paramater);
	}
	extern void create();
	extern void destroy();
}