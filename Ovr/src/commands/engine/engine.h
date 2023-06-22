#pragma once
#include "commands/manager/manager.h"

namespace commands {
	class player {
	public:
		std::string m_name{};
		uint16_t m_index{};
		CNetGamePlayer* m_gamePlayer{};
		rage::netPlayer* m_netPlayer{};
		rage::rlGamerInfo* m_gamerInfo{};
	};
	class engine {
	public:
		player getPlayerForCommandArgument(std::string arg);
		void executeWithCommand(abstractCommand*& Command, std::string context);
		bool execute(std::string& string);
		template <typename ...T>
		bool primitiveExecute(std::string fmt, T... args) {
			std::string buf{ std::vformat(fmt, std::make_format_args(args...)) };
			return execute(buf);
		}
		void replaceCommand(abstractCommand* command);
		std::vector<abstractCommand*> findMatches(std::string command);
		abstractCommand* getCommand(std::string search);
		template <typename t>
		t convertData(std::string str);
		bool m_useDirectMatchResults{};
		bool m_autoComplete{};
		bool m_useFirstResultOnTooManyResults{};
	};
	inline engine g_engine{};
}