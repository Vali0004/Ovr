#pragma once
#include "pch/pch.h"

class notify {
public:
	notify(std::string title, std::string message, std::chrono::time_point<std::chrono::system_clock> created) : m_title(title), m_message(message), m_created(created) {}
	notify(std::string title, std::string message) : notify(title, message, std::chrono::system_clock::now()) {}
public:
	bool title() {
		return !m_title.empty() && m_title.size();
	}
	bool message() {
		return !m_message.empty() && m_message.size();
	}
public:
	std::string m_title{};
	std::string m_message{};
	std::chrono::time_point<std::chrono::system_clock> m_created{};
	float m_destroyIn{ 6000.f };
	float m_alpha{ 1.f };
};
class notifications {
public:
	void draw();
	template <typename ...a>
	void add(std::string title, std::string fmt, a... args) {
		std::string message{ std::vformat(fmt, std::make_format_args(args...)) };
		m_notifications.push_back(notify(title, message));
	}
private:
	std::vector<notify> get() {
		std::vector<notify> data{};
		std::vector<u64> size{};
		//for (u64 i{}; i != m_notifications.size(); ++i) {
		//	notify& n{ m_notifications[i] };
		//	std::chrono::time_point<std::chrono::system_clock> time{ std::chrono::system_clock::now() };
		//	const float difference{ static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(time - n.m_created).count()) };
		//	n.m_alpha = 1.f;
		//	if (n.m_destroyIn <= difference) {
		//		n.m_alpha = 1.f - ((difference - n.m_destroyIn) / 600.f);
		//		n.m_alpha = n.m_alpha < 0.f ? 0.f : n.m_alpha;
		//	}
		//	if (n.m_alpha > 0.f) {
		//		data.push_back(n);
		//	}
		//	else {
		//		size.push_back(i);
		//	}
		//}
		//for (u64 i : size)
		//	m_notifications.erase(m_notifications.begin() + i);
		return data;
	}
	std::vector<notify> m_notifications{};
private:
	bool m_draw{ true };
	float m_drawBase{};
	ImGuiWindowFlags m_flags{ ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing };
	float m_width{ 340.f };
	ImVec2 m_padding{ -10.f, 20.f };
	ImU32 m_titleColor{ IM_COL32(255, 255, 255, 255) };
};
inline notifications g_notifications{};