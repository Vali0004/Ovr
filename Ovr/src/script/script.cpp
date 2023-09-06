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
#include "util/garage_mgr.h"

namespace script {
	inline bool shouldPosChange(ImVec2 pos) {
		ImVec2 winPos{ elements::windowPos() };
		return winPos.y != pos.y;
	}
	inline float g_width{ 300.f };
	inline void drawText(rage::ysc::program& p, std::string text, Vector4 color, int font, Vector2 pos, Vector2 size, bool center) {
		rage::ysc::HUD::SET_TEXT_CENTRE(p, center);
		rage::ysc::HUD::SET_TEXT_COLOUR(p, color.x, color.y, color.z, color.w);
		rage::ysc::HUD::SET_TEXT_FONT(p, font);
		rage::ysc::HUD::SET_TEXT_SCALE(p, size.x, size.y);
		//rage::ysc::HUD::SET_TEXT_DROPSHADOW(p, 1, 0, 0, 0, 0);
		//rage::ysc::HUD::SET_TEXT_EDGE(p, 1, 0, 0, 0, 0);
		rage::ysc::HUD::SET_TEXT_OUTLINE(p);
		rage::ysc::HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT(p, "STRING");
		rage::ysc::HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(p, text.c_str());
		rage::ysc::HUD::END_TEXT_COMMAND_DISPLAY_TEXT(p, pos, NULL);
	}
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
				if (ImGui::MenuItem("Test")) {
					rage::ysc::program bytecode{ [](rage::ysc::program& p) {
						p.enter("main", 0, 100);
						p.label("EntryPoint");
						rage::ysc::GRAPHICS::DRAW_RECT(p, { 0.5f, 0.5f }, 0.1f, 0.1f, 255, 192, 255, 255, FALSE);
						drawText(p, "Hello!", { 255, 255, 255, 255 }, 7, { 0.5f, 0.5f }, { 1.f, 1.f }, true);
						rage::ysc::SYSTEM::WAIT(p, 0);
						p.jmp("EntryPoint");
						p.leave(0, 0);
					} };
					rage::scrProgram program{};
					rage::scrThread::Serialised serialised{};
					serialised.m_script_hash = "dummy"_joaat;
					program.m_name = "dummy";
					program.m_name_hash = "dummy"_joaat;
					serialised.m_state = rage::eThreadState::halted;
					serialised.m_pointer_count = NULL;
					serialised.m_catch_pointer_count = NULL;
					serialised.m_frame_pointer = NULL;
					serialised.m_catch_frame_pointer = NULL;
					for (uint32_t i{}; i != bytecode.m_codePageCollection.codePages.size(); ++i) {
						program.m_code_blocks = new u8*[]();
						program.m_code_blocks[i] = new u8[]();
					}
					for (uint32_t i{}; i != bytecode.m_stringPageCollection.stringHeaps.size(); ++i) {
						program.m_string_heaps = new const char*[]();
						program.m_string_heaps[i] = new const char[]();
					}
					for (uint32_t i{}; i != bytecode.m_codePageCollection.codePages.size(); ++i) {
						memcpy(program.m_code_blocks[i], bytecode.m_codePageCollection.codePages[i], bytecode.m_codePageCollection.getCodePageSize(i));
					}
					program.m_code_size = bytecode.m_codePageCollection.getCodePageSize();
					for (uint32_t i{}; i != bytecode.m_stringPageCollection.stringHeaps.size(); ++i) {
						memcpy(const_cast<char*>(program.m_string_heaps[i]), bytecode.m_stringPageCollection.stringHeaps[i], bytecode.m_stringPageCollection.getStringChunkSize(i));
					}
					program.m_string_heap_size = bytecode.m_stringPageCollection.getStringHeapSize();
					for (uint32_t i{}; i != bytecode.m_natives.size(); ++i) {
						program.m_natives[i] = g_invoker.getNativeCmd(bytecode.m_natives[i]);
					}
					program.m_native_count = static_cast<u32>(bytecode.m_natives.size());
					program.m_global_count = 0x1337;
					if (bytecode.m_staticCount) {
						program.m_local_count = bytecode.m_staticCount;
					}
					if (bytecode.m_stackSize) {
						serialised.m_stack_size = bytecode.m_stackSize;
					}
					else {
						serialised.m_stack_size = 1024;
					}
					serialised.m_state = rage::eThreadState::running;
					rage::scrValue* stack{ new rage::scrValue[serialised.m_stack_size]() };
					pointers::g_scriptVm(stack, pointers::g_globals, &program, &serialised);
				}
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