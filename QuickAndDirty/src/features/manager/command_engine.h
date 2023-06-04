#pragma once
#include "features/manager/manager.h"

namespace features::cmd {
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
		void executeWithFeature(abstractFeature*& feature, std::string context);
		void execute(std::string& string);
		template <typename ...T>
		void primitiveExecute(std::string fmt, T... args) {
			std::string buf{ std::vformat(fmt, std::make_format_args(args...)) };
			execute(buf);
		}
		void replaceFeature(abstractFeature* feature);
	public:
		bool m_useDirectMatchResults{ true };
		bool m_autoComplete{ true };
		bool m_useFirstResultOnTooManyResults{};
	private:
		std::vector<abstractFeature*> findMatches(std::string feature);
		abstractFeature* getFeature(std::string search);
		template <typename t>
		t convertData(std::string str);
	};
	inline engine g_engine{};
}