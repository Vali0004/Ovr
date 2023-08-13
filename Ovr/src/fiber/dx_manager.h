#pragma once
#include "pch/pch.h"
#include "rage/engine.h"
#include "fiber.h"

class dxManager {
public:
	void add(cc* id, fnptr<void()> fn) {
		std::lock_guard lck(m_mutex);
		m_fibers.insert({ id, MakeSmartPointer<fiber>(fn) });
	}
	void destroy() {
		std::lock_guard lck(m_mutex);
		m_fibers.clear();
	}
	void run() {
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
inline dxManager g_dxManager{};