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
#include "tabs/miscellaneous.h"
#include "tabs/recovery.h"
#include "tabs/settings.h"
#include "elements.h"
#include "commands/gui/gui.h"
#include "hooking/hooking.h"
#include "rage/script/loader.h"
#include "rage/script/commands.h"

namespace script {
	inline bool shouldPosChange(ImVec2 pos) {
		ImVec2 winPos{ elements::windowPos() };
		return winPos.y != pos.y;
	}
	inline float g_width{ 300.f };
	void onPresent() {
		if (script::g_guiOpen) {
			elements::window(BRAND"Header", g_guiOpen, [] {
				elements::setWindowPos({ 13.f, 100.f }, ImGuiCond_Once);
				elements::setWindowSize({ g_width, 80.f });
			}, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);
			ImGuiWindow* window{ elements::getWindow(BRAND"Header") };
			ImVec2 windowPos{ elements::getWindowPos(window) };
			ImVec2 windowSize{ elements::getWindowSize(window) };
			elements::window(BRAND, g_guiOpen, [windowPos, windowSize] {
				ImVec2 pos{ windowPos.x, windowPos.y + windowSize.y };
				elements::setWindowPos(pos);
				elements::setWindowSize({ g_width, 270.f }, ImGuiCond_Once);
				tabs::self::tab();
				tabs::weapon::tab();
				tabs::vehicle::tab();
				tabs::network::tab();
				tabs::protections::tab();
				tabs::scripting::tab();
				tabs::miscellaneous::tab();
				tabs::recovery::tab();
				tabs::settings::tab();
			}, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing);
		}
		elements::drawlist::text(g_renderer->m_tahoma, BRAND " Developer", { 0.001f, 0.f }, { 255, 255, 255, 255 });
	}
	void init() {
		if (NETWORK::NETWORK_IS_SESSION_ACTIVE()) {
			if (NETWORK::NETWORK_SESSION_IS_PRIVATE()) {
				g_sessionType = eSessionTypes::InviteOnly;
			}
			else if (NETWORK::NETWORK_SESSION_IS_CLOSED_FRIENDS()) {
				g_sessionType = eSessionTypes::FriendsOnly;
			}
			else if (NETWORK::NETWORK_SESSION_IS_CLOSED_CREW()) {
				g_sessionType = eSessionTypes::CrewOnly;
			}
			else if (NETWORK::NETWORK_SESSION_IS_SOLO()) {
				g_sessionType = eSessionTypes::Solo;
			}
			else if (NETWORK::NETWORK_SESSION_IS_VISIBLE()) {
				g_sessionType = eSessionTypes::Public;
			}
			else {
				g_sessionType = eSessionTypes::CrewSession;
			}
		}
		else {
			g_sessionType = eSessionTypes::Offline;
		}
		g_statistics.setGameType();
		util::vehicle::cacheModelTable();
	}
	void onTick() {
		g_notifications.add("Welocme", "Welcome to " BRAND "! You are using version 0.00.1");
		#ifdef DEBUG
		g_notifications.add("User Checks", "Welcome, Vali!");
		#else
		g_notifications.add("User Checks", "Welcome, {}!", util::network::socialclub::getString("gtag"));
		#endif
		init();
		while (true) {
			if (g_guiOpen || commands::gui::g_box.m_lock || g_renderer->hasActiveCallback()) {
				PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
			}
			if (g_sessionType != eSessionTypes::Offline && !util::network::g_manager.online()) {
				g_sessionType = eSessionTypes::Offline;
				g_statistics.setGameType();
			}
			if ((NETWORK::NETWORK_TEXT_CHAT_IS_TYPING() || HUD::IS_MP_TEXT_CHAT_TYPING()) && !NETWORK::NETWORK_IS_IN_TRANSITION()) {
				commands::gui::g_box.m_canUseBox = false;
			}
			else {
				commands::gui::g_box.m_canUseBox = true;
			}
			fiber::current()->sleep();
		}
	}
	void presentInit() {
		auto& style{ ImGui::GetStyle() };
		auto& colors{ style.Colors };
		auto& io{ ImGui::GetIO() };
		style.WindowTitleAlign = { 0.5f, 0.5f };
		style.WindowRounding = 5.5f;
		style.ChildRounding = 0.1f;
		style.FrameRounding = 4.f;
		style.GrabRounding = 3.f;
		style.PopupRounding = 5.5f;
		style.ScrollbarRounding = 12.f;
		style.ScrollbarSize = 9.f;
		style.FrameBorderSize = 0.f;
		style.WindowBorderSize = 1.f;
		style.IndentSpacing = 27.f;
		style.AntiAliasedLines = true;
		style.AntiAliasedFill = true;
		colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_TextSelectedBg] = ImColor(49, 151, 253, 255);
		colors[ImGuiCol_TextDisabled] = ImColor(40, 40, 40, 255);
		colors[ImGuiCol_WindowBg] = ImColor(15, 15, 15, 255);
		colors[ImGuiCol_ChildBg] = ImColor(20, 20, 20, 255);
		colors[ImGuiCol_PopupBg] = ImColor(20, 20, 20, 255);
		colors[ImGuiCol_Border] = ImColor(255, 95, 140, 255);
		colors[ImGuiCol_BorderShadow] = ImColor(41, 41, 41, 255);
		colors[ImGuiCol_FrameBg] = ImColor(41, 41, 41, 255);
		colors[ImGuiCol_FrameBgHovered] = ImColor(41, 41, 41, 255);
		colors[ImGuiCol_FrameBgActive] = ImColor(41, 41, 41, 255);
		colors[ImGuiCol_TitleBg] = ImColor(43, 43, 43, 255);
		colors[ImGuiCol_TitleBgActive] = ImColor(49, 49, 49, 255);
		colors[ImGuiCol_TitleBgCollapsed] = ImColor(0, 0, 0, 130);
		colors[ImGuiCol_MenuBarBg] = ImColor(30, 30, 30, 255);
		colors[ImGuiCol_ScrollbarBg] = ImColor(50, 50, 50, 255);
		colors[ImGuiCol_ScrollbarGrab] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_ScrollbarGrabActive] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_SliderGrab] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_SliderGrabActive] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_Button] = ImColor(41, 41, 41, 255);
		colors[ImGuiCol_ButtonHovered] = ImColor(41, 41, 41, 255);
		colors[ImGuiCol_ButtonActive] = ImColor(41, 41, 41, 255);
		colors[ImGuiCol_Header] = ImColor(94, 94, 94, 130);
		colors[ImGuiCol_HeaderHovered] = ImColor(97, 97, 97, 94);
		colors[ImGuiCol_HeaderActive] = ImColor(94, 94, 94, 130);
		colors[ImGuiCol_Separator] = ImColor(41, 41, 41, 255);
		colors[ImGuiCol_SeparatorHovered] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_SeparatorActive] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_ResizeGrip] = ImColor(66, 66, 66, 255);
		colors[ImGuiCol_ResizeGripHovered] = ImColor(80, 80, 80, 255);
		colors[ImGuiCol_ResizeGripActive] = ImColor(90, 90, 90, 255);
		colors[ImGuiCol_Tab] = ImColor(50, 50, 50, 255);
		colors[ImGuiCol_TabHovered] = ImColor(80, 80, 80, 255);
		colors[ImGuiCol_TabActive] = ImColor(80, 80, 80, 255);
		colors[ImGuiCol_TabUnfocused] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_TabUnfocusedActive] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_PlotLines] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_PlotLinesHovered] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_PlotHistogram] = ImColor(230, 179, 0, 255);
		colors[ImGuiCol_PlotHistogramHovered] = ImColor(255, 153, 0, 255);
		colors[ImGuiCol_DragDropTarget] = ImColor(255, 255, 0, 230);
		colors[ImGuiCol_NavHighlight] = ImColor(255, 255, 255, 255);
		colors[ImGuiCol_NavWindowingHighlight] = ImColor(255, 255, 255, 179);
		colors[ImGuiCol_NavWindowingDimBg] = ImColor(204, 204, 204, 50);
		colors[ImGuiCol_ModalWindowDimBg] = ImColor(204, 204, 204, 90);
	}
}