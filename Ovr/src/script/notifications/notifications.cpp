#include "pch/pch.h"
#include "notifications.h"
#include "script/elements.h"
#include "renderer/renderer.h"

void notifications::drawNotification(const std::string& title, const std::string& message) {
	elements::drawlist::rect({ m_pos.x, m_drawBase + (m_size.y / 2.f) }, m_size, { m_color });
	float titleTextHeight{ elements::getTextHeight(g_renderer->m_fontBold) };
	float textHeight{ elements::getTextHeight(g_renderer->m_font) };
	//TODO, fix this shit lmfao
	elements::drawlist::text(g_renderer->m_font, message, { m_pos.x - (m_size.x / 2.05f), m_drawBase + (m_size.y / 1.9f) - (textHeight / 2.f) }, { 255, 255, 255, 255 });
	m_drawBase -= textHeight;
	m_drawBase -= m_padding;
	elements::drawlist::text(g_renderer->m_fontBold, title, { m_pos.x - (m_size.x / 2.05f), m_drawBase + (m_size.y / 1.85f) - (titleTextHeight / 2.f) }, { 255, 255, 255, 255 });
	m_drawBase -= m_size.y;
}
void notifications::draw() {
	if (m_draw) {
		m_drawBase = m_pos.y;
		updateArray();
		/*for (auto& pair : m_notifications) {
			auto& n{ pair.second };
			drawNotification(n.m_title, n.m_message);
		}*/
	}
}