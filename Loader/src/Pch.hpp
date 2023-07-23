#pragma once
#ifndef PCH_H
#define PCH_H

#define _HAS_STD_BYTE 0
#include "Framework.hpp"
#define BRAND "Ovr"
#define LOG(T, F, ...) printf("%s\n", std::string(#T + std::string(" | ") + std::vformat(F, std::make_format_args(__VA_ARGS__))).c_str());

namespace Defines {
	inline bool g_Running{ true };
	inline std::vector<std::string> g_Log{};
	inline MSG g_Msg{};
	template <typename ...A>
	inline int cmd(std::string Command, bool Shell, A&&... Args) {
		if (Shell) {
			return ShellExecuteA(0, "open", std::vformat(Command, std::make_format_args(Args...)).c_str(), NULL, NULL, SW_SHOW) ? 1 : 0;
		}
		else {
			return system(std::vformat(Command, std::make_format_args(Args...)).c_str());
		}
		return NULL;
	}
}

using namespace Defines;

#endif //PCH_H