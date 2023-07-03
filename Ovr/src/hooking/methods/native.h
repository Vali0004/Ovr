#pragma once
#include "pch/pch.h"
#include "rage/commands/invoker/invoker.h"

inline std::pair<bool, std::vector<class native*>> g_nativeHooks{ true, {} };
class native {
public:
	native(u32 script, u64 hash, rage::Cmd replacement) : m_script(script), m_hash(hash), m_cmd(g_invoker.getNativeCmd(m_hash)), m_replacement(replacement) {}
public:
	void enable() {
		m_enabled = true;
	}
	void disable() {
		m_enabled = false;
	}
	void set(rage::scrProgram* program, u32 nativeIdx, rage::Cmd& cmd) {
		if (bool enabled{ g_nativeHooks.first || m_enabled }; enabled && m_script == "all_scripts"_joaat || program->m_name_hash == m_script) {
			if (program->m_natives[nativeIdx] == m_cmd) {
				cmd = m_replacement;
			}
		}
	}
private:
	bool m_enabled{};
	u32 m_script{};
	u64 m_hash{};
	rage::Cmd m_cmd{};
	rage::Cmd m_replacement{};
};