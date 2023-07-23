#pragma once
#include "Pch.hpp"

namespace Elements {
	inline void Dummy(ImVec2 Size) {
		ImGui::Dummy(Size);
	}
	inline void PushWidth(float Size) {
		ImGui::PushItemWidth(Size);
	}
	inline void SameLine() {
		ImGui::SameLine();
	}
	inline void SetWindowPos(ImVec2 Value, bool Once) {
		ImGui::SetNextWindowPos(Value);
	}
	inline void SetWindowSize(ImVec2 Value, bool Once) {
		ImGui::SetNextWindowSize(Value, Once);
	}
	inline void SetWindow(ImVec2 Pos, ImVec2 Size, bool Once) {
		SetWindowPos(Pos, Once);
		SetWindowSize(Size, Once);
	}
	inline ImVec2 GetWindowsPos() {
		return ImGui::GetWindowPos();
	}
	inline ImGuiWindow* GetWindow() {
		return ImGui::GetCurrentWindow();
	}
	inline ImDrawList* GetWindowDrawlist() {
		return GetWindow()->DrawList;
	}
	inline void AddWindowRect(ImVec2 Size, ImColor Color) {
		ImVec2 Pos{ GetWindowsPos() };
		GetWindowDrawlist()->AddRectFilled(Pos, Pos + Size, Color);
	}
	inline void PopStyleVars(int Count = 1) {
		ImGui::PopStyleVar(Count);
	}
	inline void SetStyleVars(std::vector<std::pair<ImGuiStyleVar_, ImVec2>> Vars, std::function<void()> Cb = {}) {
		for (auto& Var : Vars) {
			ImGui::PushStyleVar(Var.first, Var.second);
		}
		Cb();
		PopStyleVars(Vars.size());
	}
	inline void SetStyleVars(std::vector<std::pair<ImGuiStyleVar_, float>> Vars, std::function<void()> Cb = {}) {
		for (auto& Var : Vars) {
			ImGui::PushStyleVar(Var.first, Var.second);
		}
		Cb();
		PopStyleVars(Vars.size());
	}
	inline void SetStyleColor(std::vector<std::pair<ImGuiCol_, ImU32>> Vars, std::function<void()> Cb = {}) {
		for (auto& Var : Vars) {
			ImGui::PushStyleColor(Var.first, Var.second);
		}
		Cb();
		ImGui::PopStyleColor(Vars.size());
	}
	inline void SetNextItemWidth(float Width) {
		ImGui::SetNextItemWidth(Width);
	}
	inline void SetItemFocus() {
		ImGui::SetItemDefaultFocus();
	}
	inline void Separator() {
		ImGui::Separator();
	}
	inline void ItemWidth(float Width, std::function<void()> Cb = {}) {
		ImGui::PushItemWidth(Width);
		if (Cb) {
			Cb();
		}
		ImGui::PopItemWidth();
	}
	inline void ItemId(int Id, std::function<void()> Cb = {}) {
		ImGui::PushID(Id);
		if (Cb) {
			Cb();
		}
		ImGui::PopID();
	}
	template <typename ...T>
	inline void Text(std::string Fmt, T... Args) {
		std::string Str{ std::vformat(Fmt, std::make_format_args(Args...)) };
		ImGui::Text(Str.c_str());
	}
	inline void Window(std::string Title, bool& Value, std::function<void()> Cb = {}, ImGuiWindowFlags Flags = NULL) {
		if (ImGui::Begin(Title.c_str(), &Value, Flags)) {
			if (Cb) {
				Cb();
			}
			ImGui::End();
		}
	}
	inline void PrimitiveWindow(std::string Title, std::function<void()> Cb = {}) {
		static bool T{ true };
		Window(Title, T, Cb, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
	}
	inline void ListBox(std::string Label, ImVec2 Size = {}, std::function<void()> Cb = {}, bool ContinueLine = false) {
		if (ImGui::BeginListBox(std::string("##" + Label).c_str(), Size)) {
			if (Cb) {
				Cb();
			}
			ImGui::EndListBox();
		}
		if (ContinueLine)
			SameLine();
	}
	inline void Child(std::string Label, ImVec2 Size = {}, bool Border = false, std::function<void()> Cb = {}, ImGuiWindowFlags Flags = NULL, bool ContinueLine = false) {
		if (ImGui::BeginChild(Label.c_str(), Size, Border, Flags)) {
			if (Cb) {
				Cb();
			}
			ImGui::EndChild();
		}
		if (ContinueLine)
			SameLine();
	}
	inline void PrimitiveChild(std::string Id, ImVec2 Size = {}, std::function<void()> Cb = {}, bool ContinueLine = false) {
		Child("##" + Id, Size, false, Cb, ImGuiWindowFlags_NoBackground, ContinueLine);
	}
	inline void TabBar(std::string Id, std::function<void()> Cb = {}) {
		if (ImGui::BeginTabBar(Id.c_str())) {
			if (Cb) {
				Cb();
			}
			ImGui::EndTabBar();
		}
	}
	inline void TabItem(std::string Name, std::function<void()> Cb = {}) {
		if (ImGui::BeginTabItem(Name.c_str())) {
			PrimitiveChild(Name, {}, [&] {
				if (Cb) {
					Cb();
				}
			});
			ImGui::EndTabItem();
		}
	}
	inline void Section(std::string Label, std::function<void()> Cb = {}, bool ContinueLine = false) {
		if (ImGui::TreeNode(Label.c_str())) {
			if (Cb) {
				Cb();
			}
			ImGui::TreePop();
		}
		if (ContinueLine)
			SameLine();
	}
	inline void Checkbox(std::string Label, bool& Value, std::function<void()> Cb = {}, bool ContinueLine = false) {
		if (ImGui::Checkbox(Label.c_str(), &Value)) {
			if (Cb) {
				Cb();
			}
		}
		if (ContinueLine)
			SameLine();
	}
	inline void Selectable(std::string Label, bool Selected, std::function<void()> Cb = {}, bool ContinueLine = false) {
		if (ImGui::Selectable(Label.c_str(), Selected)) {
			if (Cb) {
				Cb();
			}
		}
		if (Selected)
			SetItemFocus();
		if (ContinueLine)
			SameLine();
	}
	inline void Combo(std::string Label, std::string Preview, std::function<void()> Cb = {}, bool ContinueLine = false) {
		if (ImGui::BeginCombo(Label.c_str(), Preview.c_str())) {
			Cb();
			ImGui::EndCombo();
		}
		if (ContinueLine)
			SameLine();
	}
	inline void Button(std::string Label, std::function<void()> Cb = {}, bool ContinueLine = false) {
		if (ImGui::Button(Label.c_str())) {
			if (Cb) {
				Cb();
			}
		}
		if (ContinueLine)
			SameLine();
	}
	inline void IntSlider(std::string Label, int& Value, int Min, int Max, std::function<void()> Cb = {}, bool ContinueLine = false) {
		if (ImGui::SliderInt(Label.c_str(), &Value, Min, Max)) {
			if (Cb) {
				Cb();
			}
		}
		if (ContinueLine)
			SameLine();
	}
	inline void FloatSlider(std::string Label, float& Value, float Min, float Max, std::function<void()> Cb = {}, bool ContinueLine = false) {
		if (ImGui::SliderFloat(Label.c_str(), &Value, Min, Max, "%.2f")) {
			if (Cb) {
				Cb();
			}
		}
		if (ContinueLine)
			SameLine();
	}
	inline void ToggleIntSlider(std::string Label, bool& Toggle, int& Value, int Min, int Max, float Width = 200.f, std::function<void()> Cb = {}, bool ContinueLine = false) {
		PushWidth(Width);
		Checkbox("##" + Label, Toggle, Cb, true);
		IntSlider(Label, Value, Min, Max, {}, ContinueLine);
	}
	inline void ToggleFloatSlider(std::string Label, bool& Toggle, float& Value, float Min, float Max, float Width = 200.f, std::function<void()> Cb = {}, bool ContinueLine = false) {
		PushWidth(Width);
		Checkbox("##" + Label, Toggle, Cb, true);
		FloatSlider(Label, Value, Min, Max, {}, ContinueLine);
	}
	inline void InputInt(std::string Label, int& Value, bool ContinueLine = false) {
		ImGui::InputInt(Label.c_str(), &Value);
		if (ContinueLine)
			SameLine();
	}
	template <size_t Size>
	inline void InputText(std::string Label, char(&Output)[Size], bool Hidden = false, bool NoBlank = false, float Width = -1.f, std::function<void()> Cb = {}, bool ContinueLine = false) {
		SetNextItemWidth(Width);
		ImGuiInputTextFlags Flags{};
		if (NoBlank)
			Flags |= ImGuiInputTextFlags_CharsNoBlank;
		if (Hidden)
			Flags |= ImGuiInputTextFlags_Password;
		if (ImGui::InputTextWithHint(("##_" + Label).c_str(), Label.c_str(), Output, Size, Flags)) {
			if (Cb) {
				Cb();
			}
		}
		if (ContinueLine)
			SameLine();
	}
}