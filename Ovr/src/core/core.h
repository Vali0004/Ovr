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
	struct scyllaHide {
		void load() {
			fs::path path{ std::getenv("appdata") };
			path /= BRAND;
			path /= "Modules"; 
			path /= "ScyllaHide.dll";
			m_hmodule = LoadLibraryA(path.string().c_str());
		}
		void unload() {
			if (m_hmodule)
				FreeLibrary(m_hmodule);
		}
		HMODULE m_hmodule{};
	};
	namespace thread {
		extern void create(HMODULE hmodule);
		extern void loop();
		extern void destroy();
		extern DWORD entry(LPVOID paramater);
	}
	extern void create();
	extern void destroy();
}