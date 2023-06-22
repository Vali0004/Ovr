#pragma once
#include "pch/pch.h"
#include "commands/manager/abstract.h"
#include "script/elements.h"

namespace elements {
	struct color {
		u8 r{}, g{}, b{}, a{};
		inline u32 pack() { return a << 24 | b << 16 | g << 8 | r << 0; }
	};
	inline ImVec2 getResolution() {
		return { ImGui::GetIO().DisplaySize };
	}
	inline ImVec2 convertCoordTypes(ImVec2 pos, bool isDC = false) {
		if (isDC) {
			return pos / getResolution();
		}
		return pos * getResolution();
	}
	inline ImVec2 getTextSize(ImFont* font, std::string text, float wrap = 0.f) {
		ImVec2 textSize{ font->CalcTextSizeA(font->FontSize, FLT_MAX, wrap, text.c_str(), NULL) };
		textSize.x = IM_FLOOR(textSize.x + 0.99999999999f);
		return { convertCoordTypes(textSize, true) };
	}
	inline float getTextHeight(ImFont* font, float wrap = 0.f) {
		ImVec2 fontSize{ getTextSize(font, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", wrap) };
		return fontSize.x;
	}
	namespace custom {
		inline void rect(ImVec2 pos, ImVec2 size, color color, bool foreground = true) {
			ImDrawList* drawList{ foreground ? ImGui::GetForegroundDrawList() : ImGui::GetBackgroundDrawList() };
			ImVec2 scaledPos{ convertCoordTypes(pos) };
			ImVec2 scaledSize{ convertCoordTypes(size) };
			ImVec2 finalPos{ scaledPos - (scaledSize / 2.f) };
			drawList->AddRectFilled(finalPos, finalPos + scaledSize, color.pack());
		}
		enum class eJustify : u8 { Left, Right, Center };
		inline void text(ImFont* font, std::string text, ImVec2 pos, color color, eJustify justify = eJustify::Left, float wrap = 0.f) {
			ImDrawList* drawList{ ImGui::GetForegroundDrawList() };
			ImVec2 scaledWrap{ convertCoordTypes({ wrap, wrap }) };
			switch (justify) {
			case eJustify::Right: {
				ImVec2 textSize{ getTextSize(font, text, scaledWrap.y) };
				pos.x -= textSize.x;
			} break;
			case eJustify::Center: {
				ImVec2 textSize{ getTextSize(font, text, scaledWrap.y) };
				pos.x -= textSize.x / 2.f;
			} break;
			}
			ImVec2 scaledPos{ convertCoordTypes(pos) };
			drawList->AddText(font, font->FontSize, scaledPos, color.pack(), text.data(), NULL, scaledWrap.y);
		}
	}
}
namespace commands::gui {
	class box {
	private:
		void captureCmd(std::string s, bool hasSpace);
		void input();
	private:
		void fetch();
		void run();
	public:
		void draw();
		void clear(bool ui = false, bool buffer = false);
		void stop();
		void alert(std::string reason = {});
	public:
		void drawItem(std::string item);
		void addItem(std::string item);
	public:
		bool m_draw{};
		bool m_lock{};
		bool m_clearCommandBoxOnEnter{ true };
	private:
		i8 m_limit{ 5 };
		i8 m_matchLimit{ 8 };
		std::string m_cmd{};
		std::string m_context{};
		std::vector<abstractCommand*> m_matches{};
		std::vector<std::string> m_items{};
	private:
		char m_inputBuffer[256]{};
		ImVec2 m_pos{ 0.5f, 0.05f };
		float m_drawBase{};
		float m_width{ 0.27f };
		ImVec2 m_title{ m_width, 0.025f };
		float m_padding{ 0.005f };
		float m_itemPadding{ 0.003f };
		ImVec2 m_inputBox{ m_width, 0.025f };
		ImVec2 m_item{ m_width, 0.025f };
	};
	inline box g_box{};
}