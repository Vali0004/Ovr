#pragma once
#include "pch/pch.h"
#include "core/logger.h"

class fiber {
public:
	fiber(fnptr<void()> fn) : m_func(fn) {
		m_fibers.first = CreateFiber(0, &fiber::routine, this);
	}
	~fiber() {
		if (m_fibers.first) {
			DeleteFiber(m_fibers.first);
		}
	}
public:
	void sleep(std::optional<std::chrono::high_resolution_clock::duration> time = std::nullopt) {
		if (time.has_value()) {
			m_time = std::chrono::high_resolution_clock::now() + time.value();
		}
		else {
			m_time = std::nullopt;
		}
		SwitchToFiber(m_fibers.second);
	}
	void tick() {
		m_fibers.second = GetCurrentFiber();
		if (!m_time.has_value() || m_time.value() <= std::chrono::high_resolution_clock::now()) {
			SwitchToFiber(m_fibers.first);
		}
	}
	static void routine(PVOID param) {
		fiber* f{ static_cast<fiber*>(param) };
		try {
			f->m_func();
		}
		catch (std::exception& except) {
			LOG(Exception, "Caught exception {} in a fiber", except.what());
		}
		while (true) {
			f->sleep();
		}
	}
	static fiber* current() {
		return static_cast<fiber*>(GetFiberData());
	}
public:
	std::pair<void*, void*> m_fibers{};
	fnptr<void()> m_func{};
	std::optional<std::chrono::high_resolution_clock::time_point> m_time{};
};