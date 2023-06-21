#include "pch/pch.h"
#include "gui.h"
#include "commands/engine/engine.h"
#include "util/util.h"
#include "renderer/renderer.h"

class character {
public:
	character(i8 key, ccp lowercase, ccp uppercase) : m_key(key), m_lowercase(lowercase), m_uppercase(uppercase) {}
	character(char key) : character(key, {}, {}) {
		char str[2]{ key, '\0' };
		m_lowercase = str;
		str[0] = (char)toupper(str[0]);
		m_uppercase = str;
	}
private:
	i8 m_key{};
	ccp m_lowercase{};
	ccp m_uppercase{};
};
namespace commands::gui {
	#define CMD_ALERT(fmt, ...) alert(std::vformat(fmt, std::make_format_args(__VA_ARGS__)));
	#define WHITE(A) { 255, 255, 255, A }
	#define BLACK(A) { 0, 0, 0, A }
	void input(std::string& str) {

	}
	void box::init() {
		if (m_context.empty()) {
			return CMD_ALERT("no command");
		}
		std::vector<std::string> words{ g_splitStr(m_context, ' ') };
		m_matches = g_engine.findMatches(words[0]);
		if (m_matches.empty()) {
			return CMD_ALERT("no match ({})", words[0]);
		}
		for (auto& match : m_matches) {
			addItem(match->m_name);
		}
	}
	void box::draw() {
		util::onPress('U', [this] { m_draw ^= true; });
		if (m_draw) {
			util::onPress(VK_RETURN, [this] { stop(); });
			m_drawBase = m_pos.y;
			elements::custom::rect({ m_pos.x, m_drawBase + (m_title.y / 2.f) }, m_title, WHITE(255));
			elements::custom::text(g_renderer->m_tahoma, BRAND" Command Box", { m_pos.x - (m_title.x / 2.05f), m_drawBase + (m_title.y / 2.f) - (elements::custom::getTextHeight(g_renderer->m_arial, 0.25f) / 2.f) - 0.006f }, BLACK(255));
			m_drawBase += m_title.y;
			m_drawBase += m_padding;
			elements::custom::rect({ m_pos.x, m_drawBase + (m_inputBox.y / 2.f) }, m_inputBox, BLACK(190));
			elements::custom::text(g_renderer->m_tahoma, "dummy", { m_pos.x - (m_inputBox.x / 2.05f), m_drawBase + (m_inputBox.y / 2.f) - (elements::custom::getTextHeight(g_renderer->m_arial, 0.25f) / 2.f) - 0.006f }, WHITE(255));
			m_drawBase += m_inputBox.y;
			for (u64 i{}; i != m_items.size() && i != m_limit; ++i) {
				drawItem(i);
			}
		}
	}
	void box::stop() {
		m_draw = false;
		LOG(Info, "Command box closed");
	}
	void box::alert(std::string reason) {
		m_draw = false;
		LOG(Info, "Command box logic error, reason is {}", reason.empty() ? "unknown" : reason.c_str());
	}

	void box::drawItem(i8 index) {
		std::string& item{ m_items[index] };
		elements::custom::rect({ m_pos.x, m_drawBase + (m_item.y / 2.f) }, m_item, BLACK(190));
		elements::custom::text(g_renderer->m_tahoma, item, { m_pos.x - (m_item.x / 2.05f), m_drawBase + (m_item.y / 2.f) - (elements::custom::getTextHeight(g_renderer->m_arial, 0.25f) / 2.f) - 0.006f }, WHITE(255));
		m_drawBase += 0.03f;
	}
	void box::addItem(std::string item) {
		m_items.emplace_back(item);
	}
}