#pragma once
#include "commands/manager/manager.h"
#include "util/player_mgr.h"

namespace commands {
	class engine {
	public:
		util::network::player getPlayerForCommandArgument(std::string arg);
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