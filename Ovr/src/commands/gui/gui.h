#pragma once
#include "pch/pch.h"
#include "commands/manager/abstract.h"
#include "script/elements.h"

namespace commands::gui {
	struct clue {
		std::string name{};
		std::string description{};
	};
	class commandBoxClues {
	public:
		void add(std::string name, std::string description = {}) {
			m_clues.push_back({ name, description });
		}
		void clear() {
			m_clues.clear();
		}
	public:
		std::vector<clue> search(std::string s) {
			std::vector<clue> ret{};
			for (auto& clue : m_clues) {
				if (clue.name.find(s) != std::string::npos) {
					ret.push_back(clue);
				}
			}
			return ret;
		}
	private:
		std::vector<clue> m_clues{};
	};
	class box {
	private:
		void captureCmd(std::string s);
		void input();
	private:
		void fetch();
		void run();
	public:
		void draw();
		void clear(bool ui = false, bool buffer = false);
		void stop();
		void alert(std::string reason = {});
	private:
		void drawItem(std::string item);
		void addItem(std::string item);
	public:
		commandBoxClues m_clues{};
		bool m_draw{};
		bool m_lock{};
		bool m_clearCommandBoxOnEnter{ true };
	private:
		i8 m_limit{ 3 };
		i8 m_matchLimit{ 8 };
		std::string m_cmd{};
		std::string m_context{};
		std::vector<abstractCommand*> m_matches{};
		std::vector<std::string> m_items{};
	private:
		char m_inputBuffer[256]{};
		ImVec2 m_pos{ 0.5f, 0.06f };
		float m_drawBase{};
		float m_width{ 0.28f };
		ImVec2 m_title{ m_width, 0.025f };
		float m_padding{ 0.005f };
		float m_itemPadding{ 0.003f };
		ImVec2 m_inputBox{ m_width, 0.025f };
		ImVec2 m_item{ m_width, 0.025f };
	};
	inline box g_box{};
}