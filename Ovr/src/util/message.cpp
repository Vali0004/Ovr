#include "message.h"
#include "script/elements.h"

void messageHandler::draw() {
	m_drawBase = m_pos.y;
	for (auto& s : m_strings) {
		if (!s.blank()) {
			const ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s.str()) };
			switch (s.align()) {
			case 0: {
				elements::drawlist::text(g_renderer->m_tahoma, s.str(), { m_pos.x, m_drawBase }, m_color);
			} break;
			case 1: {
				elements::drawlist::text(g_renderer->m_tahoma, s.str(), { m_pos.x - (size.x / 2.f), m_drawBase }, m_color);
			} break;
			case 2: {
				elements::drawlist::text(g_renderer->m_tahoma, s.str(), { m_pos.x - size.x, m_drawBase }, m_color);
			} break;
			}
			m_drawBase += size.y;
		}
		else {
			m_drawBase += elements::getTextHeight(g_renderer->m_tahoma);
		}
		m_drawBase += m_padding;
	}
}