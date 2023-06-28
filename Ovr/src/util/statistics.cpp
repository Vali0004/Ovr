#include "pch/pch.h"
#include "renderer/renderer.h"
#include "statistics.h"
#include "script/elements.h"

void statistics::draw() {
	m_drawBase = m_pos.y;
	drawGameNativesInvoked();
	drawNativesInvoked();
	drawVehiclePool();
	drawPedPool();
	drawPickupPool();
	drawObjectPool();
	drawPlayerCount();
	drawIncomingNetworkEvents();
	drawFrameCount();
	drawFrameTime();
	drawSessionType();
	drawHost();
	drawScriptHost();
	drawPlayerName();
	drawLastScriptEventSender();
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

void statistics::drawGameNativesInvoked() {
	std::string s{ std::format("{} Natives Invoked", m_nativesInvoked) };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawNativesInvoked() {
	std::string s{ std::format("{} Natives Invoked By " BRAND, m_nativesInvokedByUs) };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawVehiclePool() {
	std::string s{ std::format("{} Vehicle", util::classes::getVehicleInterface()->m_count) };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawPedPool() {
	std::string s{ std::format("{} Pedestrians", util::classes::getPedInterface()->m_count) };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawPickupPool() {
	std::string s{ std::format("{} Pickups", util::classes::getPickupInterface()->m_count) };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawObjectPool() {
	std::string s{ std::format("{} Objects", util::classes::getObjectInterface()->m_count) };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawPlayerCount() {
	std::string s{ util::network::g_manager.online() ? std::format("{} Player{}", m_playerCount, m_playerCount == 1 ? "" : "s") : "1 Player"};
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawIncomingNetworkEvents() {
	std::string s{ util::network::g_manager.online() ? std::format("{} - Incoming Network Events", m_incomingNetworkEvents) : "N/A - Incoming Network Events" };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawFrameCount() {
	std::string s{ std::format("{} - Frame Count", m_frameCount) };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawFrameTime() {
	std::string s{ std::format("{} - Frame Time", m_frameTime) };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawSessionType() {
	std::string s{ std::format("{} - Session", m_gameType) };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawHost() {
	std::string s{ std::format("{} - Host", util::network::g_manager.online() ? m_host.m_name : "N/A") };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawNextHost() {
	std::string s{ std::format("{} - Next Host", util::network::g_manager.online() ? (m_playerCount > 1 ? m_nextHost.m_name : "N/A") : "N/A") };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawScriptHost() {
	std::string s{ std::format("{} - Script Host", util::network::g_manager.online() ? m_scriptHost.m_name : "N/A")  };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawPlayerName() {
	std::string s{ std::format("{} - Player Name", util::classes::getPlayerInfo()->m_gamer_info.m_name) };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}
void statistics::drawLastScriptEventSender() {
	std::string s{ std::format("{} - Last Script Event Sender", util::network::g_manager.online() ? m_lastScriptEventSender.m_name : "N/A") };
	ImVec2 size{ elements::getTextSize(g_renderer->m_tahoma, s) };
	elements::drawlist::text(g_renderer->m_tahoma, s, { m_pos.x - size.x, m_drawBase + size.y }, m_color);
	m_drawBase += elements::getTextSize(g_renderer->m_tahoma, s).y;
	m_drawBase += m_padding;
}