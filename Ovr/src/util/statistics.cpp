#include "pch/pch.h"
#include "renderer/renderer.h"
#include "statistics.h"
#include "script/elements.h"

float roundUp(float value, int decimalPlace = 0) {
	const float multiplier{ powf(10.f, static_cast<float>(decimalPlace)) };
	return std::roundf(value * multiplier) / multiplier;
}
void statistics::draw() {
	m_drawBase = m_pos.y;
	if (!commands::g_manager.getCommands().empty() && "drawNativesInvokedByGame"_TC) {
		if ("drawNativesInvokedByGame"_TC->get(0).toggle) {
			draw("{} Natives Invoked", m_nativesInvoked);
		}
		if ("drawNativesInvokedBy" BRAND ""_TC->get(0).toggle) {
			draw("{} Natives Invoked By " BRAND, m_nativesInvokedByUs);
		}
		if ("drawPoolCounts"_TC->get(0).toggle) {
			if (CVehicleInterface * inf{ util::classes::getVehicleInterface() })
				draw("{} Vehicles", inf->m_count);
			if (CPedInterface * inf{ util::classes::getPedInterface() })
				draw("{} Pedestrians", inf->m_count);
			if (CPickupInterface * inf{ util::classes::getPickupInterface() })
				draw("{} Pickups", inf->m_count);
			if (CObjectInterface * inf{ util::classes::getObjectInterface() })
				draw("{} Objects", inf->m_count);
		}
		if ("drawCoordinates"_TC->get(0).toggle) {
			draw("{} - X", util::classes::getPed()->get_position().x);
			draw("{} - Y", util::classes::getPed()->get_position().y);
			draw("{} - Z", util::classes::getPed()->get_position().z);
		}
		if ("drawHeading"_TC->get(0).toggle) {
			draw("{} - Heading", util::classes::getPed()->get_heading());
		}
		if ("drawPlayerCount"_TC->get(0).toggle) {
			draw("{} Player{}", util::network::g_manager.online() ? (m_playerCount ? m_playerCount : 0) : 1, (util::network::g_manager.online() ? (m_playerCount ? m_playerCount : 0) : 1) == 1 ? "" : "s");
		}
		if ("drawIncomingNetworkEventCount"_TC->get(0).toggle) {
			draw("{} - Incoming Network Events", util::network::g_manager.online() ? std::to_string(m_incomingNetworkEvents) : "N/A");
		}
		if ("drawFrameData"_TC->get(0).toggle) {
			draw("{} - Frame Count", m_frameCount);
			draw("{} - Frame Time", roundUp(m_frameTime, 2));
			draw("{} - Last Frame Time", getLastFrameTime());
		}
		if ("drasFramesPerSecond"_TC->get(0).toggle) {
			draw("{} - Frames Per Second", roundf(ImGui::GetIO().Framerate / 1.5f));
		}
		if ("drawSessionType"_TC->get(0).toggle) {
			draw("{} - Session", m_gameType);
		}
		if ("drawHostName"_TC->get(0).toggle) {
			draw("{} - Host", util::network::g_manager.online() ? m_host.m_name : "N/A");
		}
		if ("drawNextHostName"_TC->get(0).toggle) {
			draw("{} - Next Host", util::network::g_manager.online() ? (m_playerCount > 1 ? m_nextHost.m_name : "N/A") : "N/A");
		}
		if ("drawScriptHostName"_TC->get(0).toggle) {
			draw("{} - Script Host", util::network::g_manager.online() ? m_scriptHost.m_name : "N/A");
		}
		if ("drawLastScriptEventSenderName"_TC->get(0).toggle) {
			draw("{} - Last Script Event Sender", util::network::g_manager.online() ? m_lastScriptEventSender.m_name : "N/A");
		}
		if ("drawLocalPlayerName"_TC->get(0).toggle) {
			draw("{} - Player Name", util::network::g_manager.online() ? m_local.m_name : util::classes::getPlayerInfo()->m_gamer_info.m_name);
		}
	}
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
	m_playerCount = util::network::g_manager.m_playerCount;
	m_nativesInvoked = 0;
	m_nativesInvokedByUs = 0;
	m_incomingNetworkEvents = 0;
}

i32 statistics::getLastFrameTime() {
	return static_cast<i32>(1100 * static_cast<db>(g_statistics.m_frameTime));
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