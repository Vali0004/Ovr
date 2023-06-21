#pragma once
#include "commands/manager/types.h"

namespace commands {
	class manager {
	public:
		template<typename t>
		void add(t command) {
			m_commands.push_back(new t(command));
		}
		void remove(u32 id);
		void init();
		void tick();
		void clear();
		template <typename t>
		t* getCommand(u32 id) {
			for (auto& f : m_commands) {
				if (f->m_lookupId == id) {
					return (t*)f;
				}
			}
			return nullptr;
		}
		template <typename t>
		t* getCommand(cc* id) {
			for (auto& f : m_commands) {
				if (f->m_id == id) {
					return (t*)f;
				}
			}
			return nullptr;
		}
		std::vector<abstractCommand*>& getCommands() {
			return m_commands;
		}
	private:
		std::vector<abstractCommand*> m_commands{};
	};
	inline manager g_manager{};
}
inline commands::abstractCommand* operator ""_ABF(const char* str, size_t) {
	return commands::g_manager.getCommand<commands::abstractCommand>(str);
}
inline commands::toggleCommand* operator ""_TF(const char* str, size_t) {
	return commands::g_manager.getCommand<commands::toggleCommand>(str);
}
inline commands::intCommand* operator ""_IF(const char* str, size_t) {
	return commands::g_manager.getCommand<commands::intCommand>(str);
}
inline commands::toggleIntCommand* operator ""_TIF(const char* str, size_t) {
	return commands::g_manager.getCommand<commands::toggleIntCommand>(str);
}
inline commands::floatCommand* operator ""_FF(const char* str, size_t) {
	return commands::g_manager.getCommand<commands::floatCommand>(str);
}
inline commands::toggleFloatCommand* operator ""_TFF(const char* str, size_t) {
	return commands::g_manager.getCommand<commands::toggleFloatCommand>(str);
}
inline commands::actionCommand* operator ""_AF(const char* str, size_t) {
	return commands::g_manager.getCommand<commands::actionCommand>(str);
}
inline commands::protectionCommand* operator ""_PF(const char* str, size_t) {
	return commands::g_manager.getCommand<commands::protectionCommand>(str);
}
inline commands::variadicCommand* operator ""_VF(const char* str, size_t) {
	return commands::g_manager.getCommand<commands::variadicCommand>(str);
}