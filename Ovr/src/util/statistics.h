#pragma once
#include "pch/pch.h"
#include "util/player_mgr.h"
#include "script/color.h"

class statistics {
public:
	void draw();
public:
	void reset();
	i32 getLastFrameTime();
public:
	void setGameType();
private:
	template <typename ...a>
	void draw(std::string f, a... args);
public:
	i32 m_nativesInvoked{};
	i32 m_nativesInvokedByUs{};
	i32 m_playerCount{};
	i32 m_incomingNetworkEvents{};
	i32 m_frameCount{};
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