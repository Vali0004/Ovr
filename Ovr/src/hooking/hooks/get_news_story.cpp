#include "hooking/hooking.h"
#include "json/json.h"

class customNewsContext {
public:
	customNewsContext(const std::string& title, const std::string& headline, const std::string& subtitle, const std::string& content)
		: m_title(title), m_headline(headline), m_subtitle(subtitle), m_content(content)
	{}
	std::string m_title{};
	std::string m_headline{};
	std::string m_subtitle{};
	std::string m_content{};
};
std::vector<customNewsContext> g_newsContexts{
	{ BRAND, BRAND, LOWER_BRAND "cheats.one", "bottom text" }
};
u64 g_currentNewsContext{};
bool hooks::getNewsStory(CNetworkSCNewsStory* pStory) {
	if (nlohmann::json j{ pStory->to_json() }; !j.empty()) {
		if (g_currentNewsContext >= g_newsContexts.size()) {
			g_currentNewsContext = 0;
		}
		LOG_DEBUG("{}", j.dump());
		j["title"] = g_newsContexts[g_currentNewsContext].m_title;
		j["headline"] = g_newsContexts[g_currentNewsContext].m_headline;
		j["subtitle"] = g_newsContexts[g_currentNewsContext].m_subtitle;
		j["content"] = g_newsContexts[g_currentNewsContext].m_content;
		pStory->from_json(j);
		g_currentNewsContext++;
	}
	RET_CALL(getNewsStory, pStory);
}