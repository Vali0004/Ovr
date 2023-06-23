#pragma once
#include "pch/pch.h"

class notify {
public:
	notify(std::string title, std::string message, std::chrono::time_point<std::chrono::system_clock> created, float destroyIn, float alpha) : m_title(title), m_message(message), m_destroyIn(destroyIn), m_alpha(alpha) {}
	notify(std::string title, std::string message, std::chrono::time_point<std::chrono::system_clock> created, float destroyIn) : notify(title, message, created, destroyIn, 1.f) {}
	notify(std::string title, std::string message, std::chrono::time_point<std::chrono::system_clock> created) : notify(title, message, created, 6000.f) {}
	notify(std::string title, std::string message) : notify(title, message, std::chrono::system_clock::now()) {}
public:
	bool valid() {
		return m_title.size() && m_message.size();
	}
public:
	std::string m_title{};
	std::string m_message{};
	std::chrono::time_point<std::chrono::system_clock> m_created{};
	float m_destroyIn{};
	float m_alpha{};
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
		for (u64 i{}; i != m_notifications.size(); ++i) {
			notify& n{ m_notifications[i] };
			std::chrono::time_point<std::chrono::system_clock> currentTime{ std::chrono::system_clock::now() };
			const float duration{ (float)std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - n.m_created).count() };
			n.m_alpha = 1.f;
			if (n.m_destroyIn <= duration) {
				n.m_alpha = 1.f - ((duration - n.m_destroyIn) / 600.f);
				n.m_alpha = n.m_alpha < 0.f ? 0.f : n.m_alpha;
			}
			if (n.m_alpha > 0.f)
				data.push_back(n);
			else
				size.push_back(i);
		}
		//for (u64 i : size)
		//	m_notifications.erase(m_notifications.begin() + i);
		return data;
	}
	std::vector<notify> m_notifications{};
private:
	bool m_draw{ true };
	//float m_drawBase{};
	ImGuiWindowFlags m_flags{ ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing };
	float m_width{ 340.f };
	ImVec2 m_padding{ -10.f, 20.f };
};
inline notifications g_notifications{};