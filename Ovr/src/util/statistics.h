#pragma once
#include "pch/pch.h"
#include "util/player_mgr.h"
#include "script/color.h"

class statistics {
public:
	void draw();
public:
	void reset();
public:
	void setGameType();
private:
	void drawGameNativesInvoked();
	void drawNativesInvoked();
	void drawVehiclePool();
	void drawPedPool();
	void drawPickupPool();
	void drawObjectPool();
	void drawPlayerCount();
	void drawIncomingNetworkEvents();
	void drawFrameCount();
	void drawFrameTime();
	void drawSessionType();
	void drawHost();
	void drawNextHost();
	void drawScriptHost();
	void drawPlayerName();
	void drawLastScriptEventSender();
public:
	int m_nativesInvoked{};
	int m_nativesInvokedByUs{};
	int m_playerCount{};
	int m_incomingNetworkEvents{};
	int m_frameCount{};
	float m_frameTime{};
	std::string m_gameType{};
	util::network::player m_lastScriptEventSender{};
	util::network::player m_host{};
	util::network::player m_nextHost{};
	util::network::player m_local{};
	util::network::player m_scriptHost{};
private:
	ImVec2 m_pos{ 0.995f, 0.f };
	elements::color m_color{ 255, 255, 255, 255 };
	float m_drawBase{};
	float m_padding{ 0.005f };
};
inline statistics g_statistics{};