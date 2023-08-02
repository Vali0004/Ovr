#include "hooking/hooking.h"
#include "renderer/renderer.h"
#include "shv/scripthookv.h"
#include "commands/gui/gui.h"
#include "script/notifications/notifications.h"
#include "util/message.h"

HRESULT hooks::present(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
	if (*pointers::g_loadingScreenState == eLoadingScreenState::Finished) {
		g_statistics.draw();
		g_notifications.draw();
		commands::gui::g_box.draw();
		g_renderer->onPresent();
	}
	else {
		static messageHandler msg{ { 0.5f, 0.45f }, { 255, 255, 255, 255 }, 0.005f };
		ONCE(init, {
			msg.clear();
			msg.add({ "Currently in a loading screen", 1 });
			msg.add({ "Assets are not ready yet, waiting for the game to load", 1 });
			msg.add({ "", 1 });
			msg.add({ "Please wait...", 1 });
		});
		msg.draw();
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	if (*pointers::g_loadingScreenState == eLoadingScreenState::Finished) {
		shv::onPresent(g_renderer.get());
	}
	if (GetAsyncKeyState(VK_F11)) {
		LOG_DEBUG("Blocked a game frame from being updated");
		return S_OK;
	}
	return VMT_CALL(DX, present, swapChain, syncInterval, flags);
}