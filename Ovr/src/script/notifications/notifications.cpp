#include "pch/pch.h"
#include "notifications.h"
#include "script/elements.h"
#include "renderer/renderer.h"

void notifications::draw() {
	if (m_draw) {
		std::vector<notify> notifications{ get() };
		const ImVec2& ViewportSize{ ImGui::GetMainViewport()->Size };
		for (u64 i{}; i != notifications.size(); ++i) {
			notify& n{ notifications[i] };
			std::string winName{ std::format("##notification{}", i) };
			//elements::setStyleVars({ { ImGuiStyleVar_WindowBorderSize, { 1.f, 0.f } }, { ImGuiStyleVar_WindowMinSize, { 300.f, 10.f } }, { ImGuiStyleVar_WindowPadding, { 10.f, 10.f } }, { ImGuiStyleVar_WindowRounding, { 4.f, 0.f } } }, [&] {
				if (ImGui::Begin(winName.c_str(), nullptr, m_flags)) {
					ImDrawList* list{ ImGui::GetWindowDrawList() };
					ImVec2 pos{ ImGui::GetWindowPos() };
					ImVec2 size{ ImGui::GetWindowSize() };
					if (n.title()) {
						list->AddText(g_renderer->m_fontBold, g_renderer->m_fontBold->FontSize, elements::shift(pos, 10.f), m_titleColor, n.m_title.c_str());
					}
					if (n.message()) {
						elements::textWrap(m_width - 5.f, [n] {
							elements::font(g_renderer->m_arial, [n] {
								elements::text(n.m_message);
							});
						});
					}
					ImGui::End();
				}
			//});
		}
	}
}