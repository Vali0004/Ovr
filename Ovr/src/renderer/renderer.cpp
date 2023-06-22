#include "renderer.h"
#include "script/script.h"
#include "util/util.h"
#include "shv/scripthookv.h"

renderer::renderer() : m_swapchain(*pointers::g_swapChain) {
	m_wndProc = WNDPROC(SetWindowLongPtrA(pointers::g_hwnd, GWLP_WNDPROC, LONG_PTR(&renderer::wndProc)));
	if (FAILED(m_swapchain->GetDevice(__uuidof(ID3D11Device), (void**)m_device.GetAddressOf())))
		throw std::runtime_error("Failed to get the D3D device!");
	m_device->GetImmediateContext(m_context.GetAddressOf());
	m_stateSaver = std::make_unique<stateSaver>(m_context.Get());
	m_commonState = std::make_unique<CommonStates>(m_device.Get());
	m_spriteBatch = std::make_unique<SpriteBatch>(m_context.Get());
	ImGui::CreateContext();
	ImGui_ImplDX11_Init(m_device.Get(), m_context.Get());
	ImGui_ImplWin32_Init(pointers::g_hwnd);
	script::presentInit();
	static auto&& io{ ImGui::GetIO() };
	strncpy_s(m_fontCfg.Name, std::format("{}, {}px", "Arial", 19).c_str(), 40);
	m_font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 19.f, &m_fontCfg);
	strncpy_s(m_fontCfg.Name, "", 40);
	strncpy_s(m_fontCfg.Name, std::format("{}, {}px", "Arial Bold", 19).c_str(), 40);
	m_fontBold = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\ArialBD.ttf", 19.f, &m_fontCfg);
	strncpy_s(m_fontCfg.Name, "", 40);
	strncpy_s(m_fontCfg.Name, std::format("{}, {}px", "Tahoma", 28).c_str(), 40);
	m_tahoma = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 36.f, &m_fontCfg);
	strncpy_s(m_fontCfg.Name, "", 40);
	strncpy_s(m_fontCfg.Name, std::format("{}, {}px", "Arial", 36).c_str(), 40);
	m_arial = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 40.f, &m_fontCfg);
	strncpy_s(m_fontCfg.Name, "", 40);
	io.FontAllowUserScaling = true;
}
renderer::~renderer() {
	shv::onRendererReset();
	m_stateSaver.reset();
	m_commonState.reset();
	m_spriteBatch.reset();
	SetWindowLongPtrA(pointers::g_hwnd, GWLP_WNDPROC, LONG_PTR(m_wndProc));
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void renderer::onPresent() {
	static auto&& style{ ImGui::GetStyle() };
	static auto&& io{ ImGui::GetIO() };
	io.FontAllowUserScaling = true;
	io.DisplayFramebufferScale = { script::g_scale, script::g_scale };
	io.FontGlobalScale = style.MouseCursorScale = script::g_scale;
	io.MouseDrawCursor = script::g_guiOpen;
	script::onPresent();
}

LRESULT renderer::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (g_renderer.get()) {
		if (ImGui::GetCurrentContext())
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		if (uMsg == WM_KEYDOWN && wParam == VK_INSERT) {
			script::g_guiOpen ^= true;
		}
		return CallWindowProcA(g_renderer->m_wndProc, hWnd, uMsg, wParam, lParam);
	}
	return -1;
}

void renderer::createTextures() {
	for (auto& e : shv::g_createTextureArray) {
		dxTexture2DWrapper t{};
		std::wstring name{ e.second.begin(), e.second.end() };
		comPtr<ID3D11ShaderResourceView>& view{ t.m_view };
		comPtr<ID3D11Resource>& resource{ t.m_resource };
		if (SUCCEEDED(CreateWICTextureFromFile(m_device.Get(), name.data(), resource.GetAddressOf(), view.GetAddressOf(), 0))) {
			t.m_resource.As(&t.m_texture);
			t.m_texture->GetDesc(&t.m_desc);
			scriptTex sTmp{};
			sTmp.id = e.first;
			sTmp.tex = t;
			shv::g_drawTextureArray.push_back(sTmp);
			reloadTex rTmp{};
			rTmp.id = e.first;
			rTmp.name = util::strToWstr(e.second);
			shv::g_reloadArray.push_back(rTmp);
		}
	}
	shv::g_createTextureArray.clear();
}
void renderer::reloadTextures() {
	for (auto& e : shv::g_reloadArray) {
		dxTexture2DWrapper t{};
		comPtr<ID3D11ShaderResourceView>& view{ t.m_view };
		comPtr<ID3D11Resource>& resource{ t.m_resource };
		if (SUCCEEDED(CreateWICTextureFromFile(m_device.Get(), e.name.data(), resource.GetAddressOf(), view.GetAddressOf(), 0))) {
			t.m_resource.As(&t.m_texture);
			t.m_texture->GetDesc(&t.m_desc);
			scriptTex sTemp{};
			sTemp.id = e.id;
			sTemp.tex = t;
			shv::g_drawTextureArray.push_back(sTemp);
		}
	}
}
int renderer::createTexture(const char* texFileName) {
	std::string fileNameStr{ texFileName };
	if (fileNameStr.at(1) == ':' || fileNameStr.at(0) == '\\') {
		shv::g_drawTextureArrayIndex++;
		shv::g_createTextureArray[shv::g_drawTextureArrayIndex] = fileNameStr;
		return shv::g_drawTextureArrayIndex;
	}
	return -1;
}
void renderer::drawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a) {
	if (shv::g_drawTextureArray.size() > id) {
		shv::g_drawTextureArray[id].tex.setProperties(id, index, level, time, sizeX, sizeY, centerX, centerY, posX, posY, rotation, screenHeightScaleFactor, r, g, b, a);
	}
}