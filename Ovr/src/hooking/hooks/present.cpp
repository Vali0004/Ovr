#include "hooking/hooking.h"
#include "renderer/renderer.h"
#include "shv/scripthookv.h"
#include "commands/gui/gui.h"
#include "script/notifications/notifications.h"

HRESULT hooks::present(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
	if (*pointers::g_loadingScreenState == eLoadingScreenState::Finished) {
		g_statistics.draw(); //Requests assets that do not exist.
	}
	else {
		elements::drawlist::text(g_renderer->m_tahoma, "Currently in a loading screen\nAssets that do not exist are being requested\n\nPlease wait...", { 0.001f, 0.005f }, { 255, 255, 255, 255 });
	}
	g_notifications.draw(); //Should we really draw in an loading screen? No. Will we? Yes.
	commands::gui::g_box.draw();
	g_renderer->onPresent();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	if (*pointers::g_loadingScreenState == eLoadingScreenState::Finished) {
		shv::onPresent(g_renderer.get());
	}
	return VMT_CALL(DX, present, swapChain, syncInterval, flags);
}