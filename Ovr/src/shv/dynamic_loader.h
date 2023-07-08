#pragma once
#include "pch/pch.h"
#include "fiber/manager.h"

namespace shv {
	class dynamicLoader {
	public:
		dynamicLoader(fs::path path) : m_path(path) { load(); }
		virtual ~dynamicLoader() { free(); }
	public:
		virtual void load() {
			m_handle = LoadLibraryA(m_path.string().c_str());
		}
		virtual void free() {
			if (m_handle)
				FreeLibrary(m_handle);
		}
		virtual HMODULE getModule() {
			return m_handle;
		}
	private:
		fs::path m_path{};
		HMODULE m_handle{};
	};
	class asiModule : public dynamicLoader {
	public:
		asiModule(std::string name) : m_name(name), dynamicLoader(m_path.append(std::getenv("appdata")).append(BRAND"\\Scripts").append(m_name)) {}
		~asiModule() {}
	public:
		std::string& str() {
			return m_name;
		}
		const char* c_str() {
			return m_name.c_str();
		}
	private:
		fs::path m_path{};
		std::string m_name{};
	};
	class shvLoader : public dynamicLoader {
	public:
		shvLoader() : dynamicLoader(m_path.append(std::getenv("appdata")).append(BRAND"\\Modules").append("ScriptHookV.dll")) {}
		~shvLoader() {}
	private:
		fs::path m_path{};
	};
	inline SmartPointer<shvLoader> g_shvLoader{};
	class asiLoader {
	public:
		void load(std::string name) {
			std::lock_guard lock(m_mutex);
			m_modules.push_back(MakeSmartPointer<asiModule>(name));
		}
		void unload(std::string name) {
			std::lock_guard lock(m_mutex);
			for (size_t i{}; i != m_modules.size(); ++i) {
				if (auto& module{ m_modules[i] }; module && module.get()) {
					if (auto str{ module->str() }; str.c_str()) {
						if (!name.compare(str)) {
							std::string id{ std::format("shv:0x{:X}", reinterpret_cast<uint64_t>(module->getModule())) };
							g_manager.removeBase(id.c_str());
							module->free();
							m_modules.erase(m_modules.begin() + i);
						}
					}
				}
			}
		}
		void clear() {
			std::lock_guard lock(m_mutex);
			m_modules.clear();
		}
		bool isScriptLoaded(std::string name) {
			std::lock_guard lock(m_mutex);
			for (auto& m : m_modules) {
				if (auto str{ m->c_str() }; str) {
					if (!name.compare(str)) {
						return true;
					}
				}
			}
			return false;
		}
	public:
		std::recursive_mutex m_mutex{};
		std::vector<SmartPointer<asiModule>> m_modules{};
	};
	inline asiLoader g_asiLoader{};
}