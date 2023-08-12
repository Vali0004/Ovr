#include "hooking/hooking.h"
#include "commands/features.h"

bool hooks::processMatchmakingFind(u64* _This, u64* Unused, rage::JSONNode* pNode, i32* Unk) {
	bool ret{ CALL(processMatchmakingFind, _This, Unused, pNode, Unk) };
	/*if (commands::features::network::session::browser::backend::g_browser.active()) {
		i32 i{};
		for (rage::JSONNode* node{ pNode->GetChild("Results")->m_child }; node; node = node->m_sibling, i++) {
			const auto& values = splitString(node->GetChild("Attributes")->m_value, ',');
			commands::features::network::session::browser::backend::g_browser.sessions()[i].attributes.discriminator = std::stoi(values[2]);
			commands::features::network::session::browser::backend::g_browser.sessions()[i].attributes.playerCount = std::stoi(values[4]);
			commands::features::network::session::browser::backend::g_browser.sessions()[i].attributes.language = std::stoi(values[5]);
			commands::features::network::session::browser::backend::g_browser.sessions()[i].attributes.region = std::stoi(values[6]);
		}
	}*/
	return ret;
}