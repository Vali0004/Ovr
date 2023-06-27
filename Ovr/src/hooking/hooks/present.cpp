#include "hooking/hooking.h"
#include "renderer/renderer.h"
#include "shv/scripthookv.h"
#include "commands/gui/gui.h"
#include "script/notifications/notifications.h"

HRESULT hooks::present(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
	commands::gui::g_box.draw();
	g_notifications.draw();
	g_statistics.draw();
	g_renderer->onPresent();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	shv::onPresent(g_renderer.get());
	return VMT_CALL(DX, present, swapChain, syncInterval, flags);
}