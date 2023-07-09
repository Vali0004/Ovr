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
	class scyllaHide {
	public:
		scyllaHide() {
			fs::path m_path{ std::getenv("appdata") };
			load(m_path.append(BRAND).append("Modules").append("ScyllaHide.dll"));
		}
		virtual ~scyllaHide() {
			free();
		}
		virtual HMODULE getModule() {
			return m_module;
		}
		virtual void load(fs::path path) {
			m_module = LoadLibraryA(path.string().c_str());
		}
		virtual void free() {
			if (m_module)
				FreeLibrary(m_module);
		}
	private:
		HMODULE m_module{};
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