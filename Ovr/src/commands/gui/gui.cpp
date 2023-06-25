#include "pch/pch.h"
#include "gui.h"
#include "commands/engine/engine.h"
#include "util/util.h"
#include "renderer/renderer.h"

namespace commands::gui {
	#define CMD_ALERT(fmt, ...) alert(std::format(fmt, __VA_ARGS__));
	#define WHITE(A) { 255, 255, 255, A }
	#define BLACK(A) { 0, 0, 0, A }
	void box::captureCmd(std::string s, bool hasSpace) {
		if (!hasSpace) {
			m_cmd = s;
			return;
		}
		std::vector<std::string> words{ splitString(s, ' ') };
		m_cmd = words[0];
	}
	void box::input() {
		elements::setWindow(elements::convertCoordTypes({ m_pos.x - (m_inputBox.x / 2.f), m_drawBase - m_padding }), elements::convertCoordTypes({ m_width + 0.001f, 0.04f }));
		elements::window("Command Box Input", m_draw, [&] {
			elements::setNextItemWidth(elements::convertCoordTypes({ m_width, 0.f }).x);
			elements::setKeyboardFocusHere();
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
		if (m_matches.size() > 1) {
			if (g_engine.m_useDirectMatchResults) {
				for (auto& match : m_matches) {
					if (match->m_id == m_cmd || match->m_name == m_cmd) {
						m_items.clear();
						addItem(match->m_name);
						return;
					}
				}
			}
			else if (g_engine.m_useFirstResultOnTooManyResults) {
				if (m_matches[0]->m_id == m_cmd || m_matches[0]->m_name == m_cmd) {
					m_items.clear();
					addItem(m_matches[0]->m_name);
					return;
				}
			}
		}
		std::string biggestStr{};
		for (auto& match : m_matches) {
			if (match->m_name.size() > biggestStr.size())
				biggestStr = match->m_name;
		}
		if (m_matches.size() < m_limit) {
			for (auto& match : m_matches) {
				if (match->m_description.size()) {
					std::string text{ std::format("{} - {}", match->m_name, match->m_description) };
					if (elements::getTextSize(g_renderer->m_tahoma, text).x < m_item.x - 0.005f) {
						addItem(text);
					}
					else {
						addItem(match->m_name);
					}
				}
				else {
					addItem(match->m_name);
				}
			}
		}
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
		stop();
		m_draw = m_lock = true;
	}

	void box::draw() {
		if (!m_inputBuffer) {
			clear();
		}
		util::onPress('U', [this] { m_draw = m_lock = true; });
		if (m_draw) {
			util::onPress(VK_ESCAPE, [this] { stop(); });
			util::onPress(VK_RETURN, [this] { run(); clear(false, true); });
			m_drawBase = m_pos.y;
			elements::custom::rect({ m_pos.x, m_drawBase + (m_title.y / 2.f) }, m_title, WHITE(255), false);
			elements::custom::text(g_renderer->m_tahoma, BRAND" Command Box", { m_pos.x - (m_title.x / 2.05f), m_drawBase + (m_title.y / 2.f) - (elements::getTextHeight(g_renderer->m_arial, 0.25f) / 2.f) - 0.006f }, BLACK(255), elements::custom::eJustify::Left, 0.f, false);
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
	void box::clear(bool ui, bool buffer) {
		#define CLR(v) if (v.size() && !v.empty()) { v = {}; v.clear(); }
		if (!ui) {
			CLR(m_context);
			CLR(m_cmd);
		}
		CLR(m_matches);
		CLR(m_items);
		if (buffer) {
			ZeroMemory(m_inputBuffer, sizeof(m_inputBuffer));
		}
	}
	void box::stop() {
		g_fiberPool.add([this] {
			m_draw = false;
			clear(false, true);
			fiber::current()->sleep(100ms);
			m_lock = false;
		});
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