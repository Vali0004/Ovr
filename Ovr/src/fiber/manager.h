#pragma once
#include "pch/pch.h"
#include "rage/engine.h"
#include "fiber.h"

class manager : public engine::thread {
public:
	void add(cc* id, fnptr<void()> fn, bool log = true) {
		std::lock_guard lck(m_mutex);
		m_fibers.insert({ id, MakeSmartPointer<fiber>(fn) });
		if (log)
			LOG_DEBUG("Created fiber {}", id);
	}
	void add(cc* id, u64 count, fnptr<void()> fn) {
		for (u64 i{ count }; i; --i) {
			add(std::format("{}_{}", id, i).c_str(), fn, false);
		}
		LOG_DEBUG("Created fiber group '{}' with {} fibers", id, count);
	}
	void remove(cc* id) {
		std::lock_guard lck(m_mutex);
		m_fibers.erase(id);
	}
	void removeBase(cc* baseId) {
		std::lock_guard lck(m_mutex);
		for (auto& f : m_fibers) {
			auto fbrId{ f.first };
			if (strstr(fbrId, baseId)) {
				m_fibers.erase(fbrId);
				return;
			}
		}
	}
	void destroy() {
		std::lock_guard lck(m_mutex);
		m_fibers.clear();
	}
	void do_run() override {
		static bool ensureMainFiber = (ConvertThreadToFiber(nullptr), true);
		std::lock_guard lck(m_mutex);
		for (auto& scr : m_fibers) {
			scr.second->tick();
		}
	}
private:
	std::recursive_mutex m_mutex{};
	std::map<cc*, SmartPointer<fiber>> m_fibers{};
};
inline manager g_manager{};