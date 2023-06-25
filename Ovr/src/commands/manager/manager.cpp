#include "manager.h"

namespace commands {
	void hotkey::add_hotkey(int key) {
		m_keys.emplace_back(key);
	}
	bool hotkey::pressed() {
		if (m_enabled)
			return false;
		bool pressed{};
		for (auto& k : m_keys) {
			if (GetForegroundWindow() == pointers::g_hwnd) {
				if (GetAsyncKeyState(k) & 0x1) {
					pressed = true;
				}
			}
		}
		return pressed;
	}
	void manager::remove(u32 id) {
		for (size_t i{}; i != m_commands.size(); ++i) {
			auto& f{ m_commands[i] };
			if (f->m_lookupId == id) {
				delete f;
				m_commands.erase(m_commands.begin() + i);
			}
		}
	}
	void manager::init() {
		for (auto& f : m_commands) {
			f->init();
		}
	}
	void manager::tick() {
		for (auto& f : m_commands) {
			if (f->m_type != eCommandType::VariadicCommand) {
				if (f->m_looped) {
					f->run();
				}
				else if (f->m_hotkey.pressed()) {
					ONCE({ f->run(); });
				}
			}
			else {
				if (dynamic_cast<variadicCommand*>(f)->looped()) {
					f->run();
				}
			}
		}
	}
	void manager::clear() {
		for (auto& f : m_commands) {
			f->~abstractCommand();
			delete f;
		}
	}
}