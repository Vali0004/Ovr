#include "pch/pch.h"
#include "gui.h"
#include "commands/engine/engine.h"
#include "util/util.h"
#include "renderer/renderer.h"

namespace commands::gui {
	#define CC elements::convertCoordTypes
	#define CMD_ALERT(fmt, ...) alert(std::vformat(fmt, std::make_format_args(__VA_ARGS__)));
	#define WHITE(A) { 255, 255, 255, A }
	#define BLACK(A) { 0, 0, 0, A }
	void box::captureCmd(std::string s, bool hasSpace) {
		if (!hasSpace) {
			m_cmd = s;
			return;
		}
		if (s[0] == ' ') {
			return;
		}
		for (i32 i{}; i != s.size(); ++i) {
			if (s[i] == ' ') {
				m_cmd = s.substr(0, i);
			}
		}
	}
	void box::input() {
		elements::setWindow(CC({ m_pos.x - (m_inputBox.x / 2.f), m_drawBase - m_padding }), CC({ m_width + 0.001f, 0.04f }));
		elements::window("Command Box Input", m_draw, [&] {
			ImGui::SetNextItemWidth(CC({ m_width, 0.f }).x);
			ImGui::SetKeyboardFocusHere(0);
			elements::font(g_renderer->m_tahoma, [&] {
				elements::setStyleColor({ { ImGuiCol_FrameBg, {} } }, [&] {
					if (ImGui::InputText("##commandInput", m_inputBuffer, sizeof(m_inputBuffer))) {
						m_context = m_inputBuffer;
						clear(true);
						fetch();
					}
				});
			});
		}, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);
	}

	void box::fetch() {
		captureCmd(m_context, m_context.find(" ") != std::string::npos);
		m_matches = g_engine.findMatches(m_cmd);
		if (m_matches.size() > 1 && g_engine.m_useDirectMatchResults) {
			if (m_matches[0]->m_id == m_cmd || m_matches[0]->m_name == m_cmd) {
				addItem(m_matches[0]->m_name);
				return;
			}
		}
		for (auto& match : m_matches) {
			if (match->m_description.size()) {
				std::string text{};
				if (elements::getTextSize(g_renderer->m_tahoma, text).x < m_width - 0.005f) {
					addItem(std::format("{} - {}", match->m_name, match->m_description));
				}
				else {
					addItem(match->m_name);
				}
			}
			else {
				addItem(match->m_name);
			}
		}
		return;
	}
	void box::run() {
		if (m_matches.size() > m_matchLimit) {
			return CMD_ALERT("too many matches");
		}
		if (m_context.empty()) {
			return CMD_ALERT("no command");
		}
		if (m_matches.empty()) {		
			return CMD_ALERT("no match ({})", m_cmd);
		}
		fetch();
		g_engine.execute(m_context);
	}

	void box::draw() {
		if (!m_inputBuffer || m_inputBuffer == "") {
			clear();
		}
		util::onPress('U', [this] { m_draw = true; });
		if (m_draw) {
			util::onPress(VK_ESCAPE, [this] { stop(); });
			util::onPress(VK_BACK, [this] { if (m_context.empty()) { stop(); } });
			util::onPress(VK_RETURN, [this] { run(); });
			m_drawBase = m_pos.y;
			elements::custom::rect({ m_pos.x, m_drawBase + (m_title.y / 2.f) }, m_title, WHITE(255));
			elements::custom::text(g_renderer->m_tahoma, BRAND" Command Box", { m_pos.x - (m_title.x / 2.05f), m_drawBase + (m_title.y / 2.f) - (elements::getTextHeight(g_renderer->m_arial, 0.25f) / 2.f) - 0.006f }, BLACK(255));
			m_drawBase += m_title.y;
			m_drawBase += m_padding;
			elements::custom::rect({ m_pos.x, m_drawBase + (m_inputBox.y / 2.f) }, m_inputBox, BLACK(190), false);
			input();
			m_drawBase += m_inputBox.y;
			if (m_items.size() && m_context.empty()) {
				clear();
			}
			if (m_matches.size() > m_matchLimit && !g_engine.m_useFirstResultOnTooManyResults && g_engine.m_useDirectMatchResults) {
				drawItem(std::format("Sorry, {} is not unique enough.", m_context));
			}
			else {
				for (u64 i{}; i != m_items.size() && i <= m_limit; ++i) {
					drawItem(m_items[i]);
				}
			}
			if (m_items.size() > m_limit && g_engine.m_useFirstResultOnTooManyResults) {
				if (m_items.size() < 5) {
					drawItem(std::format("... and {} more", m_items.size()));
				}
				else {
					drawItem("... and many more");
				}
			}
		}
	}
	void box::clear(bool ui) {
		#define CLR(v) v.clear();
		if (!ui) {
			if (m_context.size()) {
				CLR(m_context);
			}
			if (m_cmd.size()) {
				CLR(m_cmd);
			}
		}
		if (m_matches.size()) {
			CLR(m_matches);
		}
		if (m_items.size()) {
			CLR(m_items);
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

	void box::drawItem(std::string item) {
		m_drawBase += m_itemPadding;
		elements::custom::rect({ m_pos.x, m_drawBase + (m_item.y / 2.f) }, m_item, BLACK(190));
		elements::custom::text(g_renderer->m_tahoma, item, { m_pos.x - (m_item.x / 2.05f), m_drawBase + (m_item.y / 2.f) - (elements::getTextHeight(g_renderer->m_arial, 0.25f) / 2.f) - 0.006f }, WHITE(255));
		m_drawBase += m_item.y;
	}
	void box::addItem(std::string item) {
		bool canPush{ true };
		for (auto& i : m_items) {
			if (i == item) {
				canPush = false;
			}
		}
		if (canPush)
			m_items.emplace_back(item);
	}
}