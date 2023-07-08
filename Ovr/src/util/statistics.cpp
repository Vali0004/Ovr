#include "pch/pch.h"
#include "renderer/renderer.h"
#include "statistics.h"
#include "script/elements.h"

void statistics::draw() {
	m_drawBase = m_pos.y;
	draw("{} Natives Invoked", m_nativesInvoked);
	draw("{} Natives Invoked By " BRAND, m_nativesInvokedByUs);
	if (CVehicleInterface* inf{ util::classes::getVehicleInterface() })
		draw("{} Vehicles", inf->m_count);
	if (CPedInterface* inf{ util::classes::getPedInterface() })
		draw("{} Pedestrians", inf->m_count);
	if (CPickupInterface * inf{ util::classes::getPickupInterface() })
		draw("{} Pickups", inf->m_count);
	if (CObjectInterface* inf{ util::classes::getObjectInterface() })
		draw("{} Objects", inf->m_count);
	draw("{} Player{}", util::network::g_manager.online() ? (m_playerCount ? m_playerCount : 0) : 1, m_playerCount == 1 ? "" : "s");
	draw("{} - Incoming Network Events", util::network::g_manager.online() ? std::to_string(m_incomingNetworkEvents) : "N/A");
	draw("{} - Frame Count", m_frameCount);
	draw("{} - Frame Time", m_frameTime);
	draw("{} - Session", m_gameType);
	draw("{} - Host", util::network::g_manager.online() ? m_host.m_name : "N/A");
	draw("{} - Next Host", util::network::g_manager.online() ? (m_playerCount > 1 ? m_nextHost.m_name : "N/A") : "N/A");
	draw("{} - Script Host", util::network::g_manager.online() ? m_scriptHost.m_name : "N/A");
	draw("{} - Last Script Event Sender", util::network::g_manager.online() ? m_lastScriptEventSender.m_name : "N/A");
	draw("{} - Player Name", util::network::g_manager.online() ? m_local.m_name : util::classes::getPlayerInfo()->m_gamer_info.m_name);
}

void statistics::reset() {
	if (m_host.m_netGamePlayer != util::network::g_manager.host().m_netGamePlayer)
		m_host = util::network::g_manager.host();
	if (m_nextHost.m_netGamePlayer != util::network::g_manager.nextHost().m_netGamePlayer)
		m_nextHost = util::network::g_manager.nextHost();
	if (m_local.m_netGamePlayer != util::network::g_manager.local().m_netGamePlayer)
		m_local = util::network::g_manager.local();
	if (m_scriptHost.m_netGamePlayer != util::network::g_manager.scriptHost().m_netGamePlayer)
		m_scriptHost = util::network::g_manager.scriptHost();
	m_nativesInvoked = 0;
	m_nativesInvokedByUs = 0;
	m_incomingNetworkEvents = 0;
}

void statistics::setGameType() {
	if (util::network::g_manager.online()) {
		std::string gstype{ gsTypeToString(g_sessionType) };
		m_gameType = gstype.c_str();
	}
	else {
		m_gameType = "Offline";
	}
}

template <typename ...a>
void statistics::draw(std::string f, a... args) {
	std::string s{ std::vformat(f, std::make_format_args(args...)) };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}