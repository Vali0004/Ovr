#include "script.h"
#include "fiber/fiber.h"
#include "fiber/manager.h"
#include "fiber/pool.h"
#include "memory/pointers.h"
#include "rage/classes.h"
#include "renderer/renderer.h"
#include "tabs/self.h"
#include "tabs/weapon.h"
#include "tabs/vehicle.h"
#include "tabs/network.h"
#include "tabs/protections.h"
#include "tabs/scripting.h"
#include "tabs/settings.h"
#include "elements.h"

namespace script {
	void onPresent() {
		if (script::g_guiOpen) {
			elements::window(BRAND, g_guiOpen, [] {
				elements::tabBar(BRAND, [] {
					elements::tabItem("Self", &tabs::self::tab);
					elements::tabItem("Weapon", &tabs::weapon::tab);
					elements::tabItem("Vehicle", &tabs::vehicle::tab);
					elements::tabItem("Network", &tabs::network::tab);
					elements::tabItem("Protections", &tabs::protections::tab);
					elements::tabItem("Scripting", &tabs::scripting::tab);
					elements::tabItem("Settings", &tabs::settings::tab);
				});
			});
		}
		elements::setWindow({ 20.f, 20.f }, { 155.f, 120.f });
		elements::primitiveWindow("Pools", [] {
			elements::text("Vehicles: {}", util::getVehicleCounts());
			elements::text("Peds: {}", util::getPedCounts());
			elements::text("Pickups: {}", util::getPedCounts());
			elements::text("Objects: {}", util::getObjectCounts());
		});
		elements::setWindow({ ImGui::GetIO().DisplaySize.x - 455.f, 5.f }, { 445.f, 40.f });
		elements::primitiveWindow("Overlay", [] {
			elements::setStyleColor({ { ImGuiCol_Border, {} } }, [] {
				elements::addWindowRect({ 435.f, 2.f }, { 255, 192, 255, 255 });
				elements::text("overseer.menu | FPS: {} | Date: {}", static_cast<u32>(ImGui::GetIO().Framerate / 1.5f), util::time("%c"));
			});
		});
	}
	void onTick() {
		init();
		while (true) {
			if (g_guiOpen) {
				PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
			}
			fiber::current()->sleep();
		}
	}
	void presentInit() {
		auto& style{ ImGui::GetStyle() };
		auto& colors{ style.Colors };
		auto& io{ ImGui::GetIO() };
		style.Alpha = 1.f;
		style.WindowTitleAlign = ImVec2(0.f, 0.5f);
		style.WindowPadding = ImVec2(10, 5);
		style.WindowRounding = 3.f;
		style.FramePadding = ImVec2(8, 5);
		style.FrameRounding = 4.f;
		style.ItemSpacing = ImVec2(10, 10);
		style.ItemInnerSpacing = ImVec2(4, 5);
		style.IndentSpacing = 27.f;
		style.ScrollbarSize = 9.f;
		style.ScrollbarRounding = 12.f;
		style.GrabMinSize = 7.f;
		style.GrabRounding = 3.f;
		style.TouchExtraPadding = ImVec2();
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.33f, 0.33f, 0.34f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.94f, 0.49f, 0.05f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.10f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.15f, 0.15f, 0.75f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.49f, 0.05f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.27f, 0.27f, 0.28f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.92f, 0.49f, 0.05f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.94f, 0.49f, 0.05f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}
	void init() {

	}
}