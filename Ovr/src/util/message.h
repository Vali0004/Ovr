#pragma once
#include "pch/pch.h"
#include "renderer/renderer.h"
#include "script/color.h"

class message {
public:
	message(const std::string& string, i8 align) : m_message(string), m_align(align) {}
public:
	std::string& str() {
		return m_message;
	}
	bool blank() {
		return m_message.empty();
	}
	i8 align() {
		return m_align;
	}
private:
	i8 m_align{};
	std::string m_message{};
};
class messageHandler {
public:
	messageHandler(const ImVec2& pos, const elements::color& color, const float padding) : m_pos(pos), m_color(color), m_padding(padding) {}

	void add(message str) {
		m_strings.push_back(str);
	}
	void draw();
	void clear() {
		m_strings.clear();
	}
private:
	std::vector<message> m_strings{};
	ImVec2 m_pos{};
	elements::color m_color{};
	float m_drawBase{};
	float m_padding{};
};