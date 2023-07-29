#include "Draw.hpp"
#include "Elements.hpp"
#include "Path.hpp"
#include "Game.hpp"

namespace Draw {
	void Init() {
		auto& io = ImGui::GetIO();
		auto&& style = ImGui::GetStyle();
		auto&& colors = style.Colors;
		style.Alpha = 1.f;
		style.WindowTitleAlign = ImVec2(0.f, 0.5f);
		style.WindowPadding = ImVec2(10, 5);
		style.WindowRounding = 15.f;
		style.FrameRounding = 15.f;
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
		colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
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
		Path::Init();
		Game::Init();
	}
	void DllPage() {
		Elements::TabItem("DLL", [] {
			Elements::TabBar("DLL", [] {
				Elements::TabItem("Injection", [] {
					Elements::InputInt("Injection delay (ms)", Game::g_InjectionDelay);
					Elements::Checkbox("Inject on startup", Game::g_InjectOnLaunch, {}, true);
					if (Game::Open()) {
						Elements::Button("Inject", &Game::Inject);
					}
					else {
						Elements::Button("Launch", &Game::Launch);
					}
				});
				Elements::TabItem("Path", [] {
					Elements::InputText("Path", Path::g_PathBuffer, false, false);
					Elements::InputText("Module", Path::g_DllBuffer, false, false);
					Elements::Button("Set", &Path::Set);
					Elements::Button("Open", &Path::Open, true);
				});
			});
		});
	}
	void GamePage() {
		Elements::TabItem("Game", [] {
			Elements::Combo("Version", Game::VersionString(), [] {
				for (size_t i{}; i != Game::g_GameVersions.size(); ++i) {
					Elements::Selectable(Game::g_GameVersions[i].data(), Game::g_Type == i, [i] {
						Game::g_Type = i;
						Game::WriteVersionData();
					});
				}
			});
			if (Game::Open()) {
				Elements::Button("Close", &Game::Close);
				Elements::Button("Restart", &Game::Restart);
			}
			else {
				Elements::Button("Launch", &Game::Launch);
			}
		});
	}
	void SettingsPage() {
		Elements::TabItem("Settings", [] {
			Elements::TabBar("Settings", [] {
				Elements::TabItem("Proccess", [] {
					Elements::Button("Exit", [] {
						fs::path Path{ std::getenv("appdata") };
						Path /= BRAND;
						Path /= "Cache";
						Path /= "Dll";
						if (std::filesystem::exists(Path) && !std::filesystem::is_empty(Path)) {
							std::filesystem::remove_all(Path);
						}
						g_Running = false;
					});
				});
				Elements::TabItem("Design", [] {
					Elements::Child("##Colors", { 0.f, 140.f }, true, [] {
						Elements::ItemWidth(-160.f, [] {
							static ImGuiStyle& Style = ImGui::GetStyle();
							auto& Colors{ Style.Colors };
							for (int32_t i{}; i != ImGuiCol_COUNT; ++i) {
								Elements::ItemId(i, [Colors, i] {
									const char* Name{ ImGui::GetStyleColorName(i) };
									ImGui::ColorEdit4("##Color", (float*)&Colors[i], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
									ImGui::SameLine(0.f, Style.ItemInnerSpacing.x);
									ImGui::TextUnformatted(Name);
								});
							}
						});
					}, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
				});
			});
		});
	}
	void MainPage() {
		Elements::TabBar("Main", [] {
			DllPage();
			GamePage();
			SettingsPage();
		});
	}
	void Tick() {
		Game::OnFind();
		Elements::SetWindow(ImGui::GetMainViewport()->GetCenter(), { 525.f, 285.f }, true);
		Elements::Window(BRAND, g_Running, [] {
			MainPage();
		}, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	}
}