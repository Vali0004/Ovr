#pragma once
#include "pch/pch.h"
#include "fiber/manager.h"

namespace shv {
	class dynamicModule {
	public:
		dynamicModule(fs::path path) {
			load(path);
		}
		virtual ~dynamicModule() {
			free();
		}
		virtual HMODULE getModule() {
			return m_module;
		}
		virtual void load(fs::path path) {
			fs::path _path{ std::getenv("appdata") };
			_path /= BRAND;
			m_module = LoadLibraryA(_path.append(path.string()).string().c_str());
		}
		virtual void free() {
			if (m_module)
				FreeLibrary(m_module);
		}
	private:
		HMODULE m_module{};
	};
	class asiModule : public dynamicModule {
	public:
		asiModule(std::string name) : m_name(name), dynamicModule(fs::path("Scripts").append(m_name)) {}
		~asiModule() {}
	public:
		std::string str() {
			return m_name;
		}
		const char* c_str() {
			return m_name.c_str();
		}
	private:
		std::string m_name{};
	};
	class shvLoader : public dynamicModule {
	public:
		shvLoader() : dynamicModule(fs::path("Modules").append("ScriptHookV.dll")) {}
		~shvLoader() {}
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