#pragma once
#include "pch/pch.h"
#include "fiber/pool.h"
#include "commands/manager/manager.h"
#include "fiber/pool.h"
#include "color.h"
#define cbutton(f, ...) elements::button(f->m_name, [] { f->run(); }, __VA_ARGS__);
#define ccheckbox(f, ...) elements::checkbox(f->m_name, f->get(0).toggle, __VA_ARGS__);
#define cintSlider(f, ...) elements::intSlider(f->m_name, f->get(0).i32, __VA_ARGS__);
#define ctoggleIntSlider(f, ...) elements::toggleIntSlider(f->m_name, f->get(0).toggle, f->get(1).i32, __VA_ARGS__);
#define ctoggleFloatSlider(f, ...) elements::toggleFloatSlider(f->m_name, f->get(0).toggle, f->get(1).floating_point, __VA_ARGS__);
#define cfloatSlider(f, ...) elements::floatSlider(f->m_name, f->get(0).floating_point, __VA_ARGS__);

namespace elements {
	inline ImVec2 shift(const ImVec2& value, float amount) {
		return { value.x + amount, value.y + amount };
	}
	inline const ImVec2& getResolution() {
		return ImGui::GetIO().DisplaySize;
	}
	inline const ImVec2& convertCoordTypes(const ImVec2& pos, bool isDC = false) {
		if (isDC) {
			return pos / getResolution();
		}
		return pos * getResolution();
	}
	inline void dummy(const ImVec2& size) {
		ImGui::Dummy(size);
	}
	inline void pushWidth(float size) {
		ImGui::PushItemWidth(size);
	}
	inline void sameLine() {
		ImGui::SameLine();
	}
	inline void setWindowPos(const ImVec2& value, ImGuiCond condition = NULL) {
		ImGui::SetWindowPos(value, condition);
	}
	inline void setNextWindowPos(const ImVec2& value, ImGuiCond condition = NULL, ImVec2 pivot = {}) {
		ImGui::SetNextWindowPos(value, condition, pivot);
	}
	inline void setWindowSize(const ImVec2& value, ImGuiCond condition = NULL) {
		ImGui::SetWindowSize(value, condition);
	}
	inline void setNextWindowSize(const ImVec2& value, ImGuiCond condition = NULL) {
		ImGui::SetNextWindowSize(value, condition);
	}
	inline void setWindow(const ImVec2& pos, const ImVec2& size) {
		setWindowPos(pos);
		setWindowSize(size);
	}
	inline void setNextWindow(const ImVec2& pos, const ImVec2& size) {
		setNextWindowPos(pos);
		setNextWindowSize(size);
	}
	inline void setCursorPos(const ImVec2& pos) {
		ImGui::SetCursorPos(pos);
	}
	inline ImVec2 cursorPos() {
		return ImGui::GetCursorPos();
	}
	inline ImVec2 windowPos() {
		return ImGui::GetWindowPos();
	}
	inline ImVec2 windowSize() {
		return ImGui::GetWindowSize();
	}
	inline ImGuiWindow* getWindow(const std::string& id) {
		return ImGui::FindWindowByName(id.c_str());
	}
	inline const ImVec2& getWindowSize(ImGuiWindow* window) {
		return window->Size;
	}
	inline const ImVec2& getWindowPos(ImGuiWindow* window) {
		return window->Pos;
	}
	inline float windowHeight() {
		return windowSize().y;
	}
	inline ImGuiWindow* getWindow() {
		return ImGui::GetCurrentWindow();
	}
	inline ImDrawList* getWindowDrawlist() {
		return getWindow()->DrawList;
	}
	inline const ImVec2& getTextSize(ImFont* font, const std::string& text, float wrap = 0.f) {
		ImVec2 textSize{ font->CalcTextSizeA(font->FontSize, FLT_MAX, wrap, text.c_str(), NULL) };
		textSize.x = IM_FLOOR(textSize.x + 0.99999999999f);
		return convertCoordTypes(textSize, true);
	}
	inline float getTextHeight(ImFont* font, float wrap = 0.f) {
		return getTextSize(font, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", wrap).y;
	}
	namespace drawlist {
		enum class eJustify : u8 { Left, Right, Center };
		inline void rect(const ImVec2& pos, const ImVec2& size, color color, bool foreground = true) {
			ImDrawList* drawList{ foreground ? ImGui::GetForegroundDrawList() : ImGui::GetBackgroundDrawList() };
			ImVec2 scaledPos{ convertCoordTypes(pos) };
			ImVec2 scaledSize{ convertCoordTypes(size) };
			ImVec2 finalPos{ scaledPos - (scaledSize / 2.f) };
			drawList->AddRectFilled(finalPos, finalPos + scaledSize, color.pack());
		}
		inline void text(ImFont* font, const std::string& text, ImVec2 pos, color color, eJustify justify = eJustify::Left, float wrap = 0.f, bool foreground = true) {
			ImDrawList* drawList{ foreground ? ImGui::GetForegroundDrawList() : ImGui::GetBackgroundDrawList() };
			const ImVec2& scaledWrap{ convertCoordTypes({ wrap, wrap }) };
			switch (justify) {
			case eJustify::Right: {
				const ImVec2& textSize{ getTextSize(font, text, scaledWrap.y) };
				pos.x -= textSize.x;
			} break;
			case eJustify::Center: {
				const ImVec2& textSize{ getTextSize(font, text, scaledWrap.y) };
				pos.x -= textSize.x / 2.f;
			} break;
			}
			const ImVec2& scaledPos{ convertCoordTypes(pos) };
			drawList->AddText(font, font->FontSize, scaledPos, color.pack(), text.data(), NULL, scaledWrap.y);
		}
	}
	inline void textWrap(float value, std::function<void()> cb = {}) {
		ImGui::PushTextWrapPos(value);
		if (cb) {
			cb();
		}
		ImGui::PopTextWrapPos();
	}
	inline void setStyleVars(std::vector<std::pair<ImGuiStyleVar_, ImVec2>> vars, std::function<void()> cb = {}) {
		for (auto& var : vars) {
			ImGui::PushStyleVar(var.first, var.second);
		}
		cb();
		ImGui::PopStyleVar(vars.size());
	}
	inline void setStyleColor(std::vector<std::pair<ImGuiCol_, ImU32>> vars, std::function<void()> cb = {}) {
		for (auto& var : vars) {
			ImGui::PushStyleColor(var.first, var.second);
		}
		cb();
		ImGui::PopStyleColor((int)vars.size());
	}
	inline void setNextItemWidth(float width) {
		ImGui::SetNextItemWidth(width);
	}
	inline void separator() {
		ImGui::Separator();
	}
	inline void openPopup(const std::string& id) {
		ImGui::OpenPopup(id.c_str());
	}
	inline void closeCurrentPopup() {
		ImGui::CloseCurrentPopup();
	}
	inline void setItemDefaultFocus() {
		ImGui::SetItemDefaultFocus();
	}
	inline void setKeyboardFocusHere() {
		ImGui::SetKeyboardFocusHere();
	}
	template <typename ...t>
	inline void text(const std::string& fmt, t... args) {
		std::string str{ std::vformat(fmt, std::make_format_args(args...)) };
		ImGui::Text(str.c_str());
	}
	template <typename ...t>
	inline void textUnformatted(const std::string& fmt, t... args) {
		std::string str{ std::vformat(fmt, std::make_format_args(args...)) };
		ImGui::TextUnformatted(str.c_str());
	}
	inline void font(ImFont* f, std::function<void()> cb = {}) {
		ImGui::PushFont(f);
		if (cb) {
			cb();
		}
		ImGui::PopFont();
	}
	inline void window(const std::string& title, bool& value, std::function<void()> cb = {}, ImGuiWindowFlags flags = NULL) {
		if (ImGui::Begin(title.c_str(), &value, flags)) {
			if (cb) {
				cb();
			}
			ImGui::End();
		}
	}
	inline void popupModal(const std::string& title, std::function<void()> cb = {}, ImGuiWindowFlags flags = NULL) {
		if (ImGui::BeginPopupModal(title.c_str(), NULL, flags)) {
			if (cb) {
				cb();
			}
			ImGui::EndPopup();
		}
	}
	inline void primitiveWindow(const std::string& title, std::function<void()> cb = {}, ImGuiWindowFlags flags = NULL) {
		static bool t{ true };
		window(title, t, cb, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
	}
	inline void listBox(const std::string& label, ImVec2 size = {}, std::function<void()> cb = {}) {
		if (ImGui::BeginListBox(std::string("##" + label).c_str(), size)) {
			if (cb) {
				cb();
			}
			ImGui::EndListBox();
		}
	}
	inline void child(const std::string& label, ImVec2 size = {}, bool border = false, std::function<void()> cb = {}, ImGuiWindowFlags flags = NULL) {
		if (ImGui::BeginChild(label.c_str(), size, border, flags)) {
			if (cb) {
				cb();
			}
			ImGui::EndChild();
		}
	}
	inline void primitiveChild(const std::string& id, ImVec2 size = {}, std::function<void()> cb = {}) {
		child("##" + id, size, false, cb, ImGuiWindowFlags_NoBackground);
	}
	inline void menu(const std::string& id, std::function<void()> cb = {}) {
		if (ImGui::BeginMenu(id.c_str())) {
			if (cb) {
				cb();
			}
			ImGui::EndMenu();
		}
	}
	inline void tabBar(const std::string& id, std::function<void()> cb = {}) {
		if (ImGui::BeginTabBar(id.c_str())) {
			if (cb) {
				cb();
			}
			ImGui::EndTabBar();
		}
	}
	inline void tabItem(const std::string& name, std::function<void()> cb = {}) {
		if (ImGui::BeginTabItem(name.c_str())) {
			primitiveChild(name, {}, [&] {
				if (cb) {
					cb();
				}
			});
			ImGui::EndTabItem();
		}
	}
	inline void section(const std::string& label, std::function<void()> cb = {}) {
		if (ImGui::TreeNode(label.c_str())) {
			if (cb) {
				cb();
			}
			ImGui::TreePop();
		}
	}
	inline void checkbox(const std::string& label, bool& value, std::function<void()> cb = {}, bool runUnderFiber = false) {
		if (ImGui::Checkbox(label.c_str(), &value)) {
			if (cb) {
				if (runUnderFiber) {
					g_pool.add(cb);
					return;
				}
				cb();
			}
		}
	}
	inline void selectable(const std::string& label, bool selected, std::function<void()> cb = {}, bool runUnderFiber = false) {
		if (ImGui::Selectable(label.c_str(), selected)) {
			if (cb) {
				if (runUnderFiber) {
					g_pool.add(cb);
					return;
				}
				cb();
			}
		}
	}
	inline void combo(const std::string& label, const std::string& preview, std::function<void()> cb = {}) {
		if (ImGui::BeginCombo(label.c_str(), preview.c_str())) {
			cb();
			ImGui::EndCombo();
		}
	}
	inline void button(const std::string& label, std::function<void()> cb = {}, ImVec2 size = {}, bool continueLine = false, bool runUnderFiber = false) {
		if (ImGui::Button(label.c_str(), size)) {
			if (cb) {
				if (runUnderFiber) {
					g_pool.add(cb);
					return;
				}
				cb();
			}
		}
		if (continueLine)
			sameLine();
	}
	inline void intSlider(const std::string& label, int& value, int min, int max, std::function<void()> cb = {}) {
		if (ImGui::SliderInt(label.c_str(), &value, min, max)) {
			if (cb) {
				cb();
			}
		}
	}
	inline void floatSlider(const std::string& label, float& value, float min, float max, std::function<void()> cb = {}) {
		if (ImGui::SliderFloat(label.c_str(), &value, min, max, "%.2f")) {
			if (cb) {
				cb();
			}
		}
	}
	inline void toggleIntSlider(const std::string& label, bool& toggle, int& value, int min, int max, float width = 200.f, std::function<void()> cb = {}, bool runUnderFiber = false) {
		pushWidth(width);
		checkbox("##" + label, toggle, cb, runUnderFiber);
		sameLine();
		intSlider(label, value, min, max);
	}
	inline void toggleFloatSlider(const std::string& label, bool& toggle, float& value, float min, float max, float width = 200.f, std::function<void()> cb = {}, bool runUnderFiber = false) {
		pushWidth(width);
		checkbox("##" + label, toggle, cb, runUnderFiber);
		sameLine();
		floatSlider(label, value, min, max);
	}
	template <u64 size>
	inline void inputText(const std::string& label, char(&output)[size], float width = -1.f, std::function<void()> cb = {}, bool hidden = false) {
		setNextItemWidth(width);
		setStyleVars({ { ImGuiStyleVar_FramePadding, { 10.f, 8.f } }, { ImGuiStyleVar_FrameRounding, { 4.f, 0.f } } }, [&] {
			if (ImGui::InputTextWithHint(("##_" + label).c_str(), label.c_str(), output, size, hidden ? ImGuiInputTextFlags_Password : ImGuiInputTextFlags_None)) {
				if (cb) {
					cb();
				}
			}
		});
	}
	inline void popup(const std::string& id, std::function<void()> cb = {}) {
		if (ImGui::BeginPopup(id.c_str())) {
			if (cb) {
				cb();
			}
			ImGui::EndPopup();
		}
	}
	inline void popupButton(const std::string& name, const std::string& id, bool continueLine = false) {
		button(name, [&] {
			openPopup(id);
		}, {}, continueLine);
	}
	template <typename t>
	inline void selectionPopup(const std::string& id, const std::string& hint, t& index, ccp* data, u8 size, std::function<void(int)> cb = {}, bool continueLine = false) {
		textUnformatted(data[static_cast<u8>(index)]);
		popup(id, [&] {
			textUnformatted(hint);
			separator();
			for (u8 i{}; i != size; ++i) {
				selectable(data[i], static_cast<u64>(index) == i, [&] {
					index = static_cast<t>(i);
					cb(i);
				});
			}
		});
		if (continueLine)
			sameLine();
	}
	inline void protectionToggle(ccp id, bool continueLine = false) {
		auto cmd{ commands::g_manager.getCommand<commands::protectionCommand>(id) };
		popupButton(cmd->m_name, id, true);
		selectionPopup<eProtectionState>(id, "State", cmd->m_state, g_protectionStates, COUNT(g_protectionStates), [cmd](int idx) {
			cmd->update(g_protectionStates[idx]);
		}, continueLine);
	}
	inline void sectionProtectionToggle(ccp id, bool continueLine = false) {
		auto cmd{ commands::g_manager.getCommand<commands::sectionProtectionCommand>(id) };
		popupButton(cmd->m_name, id, true);
		selectionPopup<eProtectionState>(id, "State", cmd->m_state, g_protectionStates, COUNT(g_protectionStates), [cmd](int idx) {
			cmd->update(g_protectionStates[idx]);
			cmd->run();
		}, continueLine);
	}
}