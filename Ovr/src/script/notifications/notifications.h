#pragma once
#include "pch/pch.h"
#include "rage/joaat.h"

class notify {
public:
	notify(std::string title, std::string message, std::chrono::time_point<std::chrono::system_clock> created) : m_title(title), m_message(message), m_created(created) {}
	notify(std::string title, std::string message) : notify(title, message, std::chrono::system_clock::now()) {}
	notify() {}
public:
	bool title() {
		return !m_title.empty() && m_title.size();
	}
	bool message() {
		return !m_message.empty() && m_message.size();
	}
	void renew() {
		m_created = std::chrono::system_clock::now();
		m_count++;
	}
public:
	std::string m_title{};
	std::string m_message{};
	std::chrono::time_point<std::chrono::system_clock> m_created{};
	float m_destroyIn{ 6000.f };
	i32 m_count{};
};
class notifications {
public:
	void draw();
	template <typename ...a>
	void add(const std::string& title, const std::string& fmt, a... args) {
		std::string message{ std::vformat(fmt, std::make_format_args(args...)) };
		u32 key{ rage::joaat(message) };
		if (exists(key)) {
			m_notifications[key].renew();
			return;
		}
		m_notifications.insert({ key, notify(title, message) });
	}
private:
	void drawNotification(const std::string& title, const std::string& message);
	void updateArray() {
		for (auto& pair : m_notifications) {
			auto& n{ pair.second };
			std::chrono::time_point<std::chrono::system_clock> time{ std::chrono::system_clock::now() };
			const float difference{ static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(time - n.m_created).count()) };
			if (n.m_destroyIn <= difference) {
				m_notifications.erase(pair.first);
			}
		}
	}
	bool exists(u32 key) {
		for (auto& pair : m_notifications) {
			if (pair.first == key) {
				return true;
			}
		}
		return false;
	}
	std::map<u32, notify> m_notifications{};
private:
	bool m_draw{ true };
	float m_drawBase{};
	ImVec2 m_size{ 0.15f, 0.03f };
	ImVec2 m_lineSize{ 0.002f, 0.035f };
	float m_padding{ 0.005f };
	ImVec2 m_pos{ 0.9542f - m_size.x, 0.99f - m_size.y };
	u32 m_color{ IM_COL32(10, 10, 10, 255) };
	u32 m_lineColor{ IM_COL32(10, 10, 10, 255) };
};
inline notifications g_notifications{};