#include "pch/pch.h"
#include "notifications.h"
#include "script/elements.h"
#include "renderer/renderer.h"

void notifications::draw() {
	float m_drawBase = 0.f;
	//std::vector<notify> notifications{ get() };
	for (size_t i{}; i != m_notifications.size(); ++i) {
		notify& n{ m_notifications[i] };
		const ImVec2 viewportSize{ ImGui::GetMainViewport()->Size };
		std::string windowName{ std::format("##notification{}", i) };
		//elements::setStyleVars({ { ImGuiStyleVar_WindowBorderSize, { 1.f, 0.f } }, { ImGuiStyleVar_WindowMinSize, { 300, 10.f } }, { ImGuiStyleVar_WindowPadding, { 10.f, 10.f } }, { ImGuiStyleVar_WindowRounding, { 4.f, 0.f } } }, [&] {
		//	elements::setStyleColor({ { ImGuiCol_Border, { IM_COL32(0, 0, 0, 0) } } }, [&] {
				elements::setWindow({ viewportSize.x + m_padding.x, viewportSize.y + m_padding.y }, { m_width, 0.f });
				elements::window(windowName, m_draw, [&] {
					ImDrawList* drawList{ ImGui::GetWindowDrawList() };
					ImVec2 pos{ elements::windowPos() };
					ImVec2 size{ elements::windowSize() };
					if (n.valid()) {
						drawList->AddText(g_renderer->m_font, 15.f, elements::shift(pos, 10.f), IM_COL32(255, 255, 255, 255), n.m_title.c_str());
						elements::textWrap(m_width - 5.f, [&] {
							elements::setCursorPos({ 10.f, 30.f });
							elements::text(n.m_message);
						});
					}
					m_drawBase += elements::windowHeight() + 6.f;
				});
			//});
		//});
	}
}