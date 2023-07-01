#include "renderer.h"
#include "script/script.h"
#include "util/util.h"
#include "shv/scripthookv.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/image.h"
#include "stb/image_write.h"

namespace stb {
	namespace memory {
		imageData write(vec2& size, int comp, const void* data, int strideBytes, uint32_t delay) {
			int imageSize;
			stbi_uc* image{ stbi_write_png_to_mem((const unsigned char*)data, strideBytes, size.x.i32, size.y.i32, comp, &imageSize) };
			RETURN_IF_VALID(image, imageSize, delay)
		}
		image read(imageData data) {
			vec2 size{};
			stbi_uc* image{ stbi_load_from_memory(data.bytes, data.size, &size.x.i32, &size.y.i32, NULL, 0) };
			RETURN_IF_VALID(image, size)
		}
	}
	namespace file {
		image readImage(fs::path path) {
			vec2 size{};
			stbi_uc* image{ stbi_load(path.string().c_str(), &size.x.i32, &size.y.i32, NULL, 4) };
			RETURN_IF_VALID(image, size)
		}
	}
}
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
	io.MouseDrawCursor = script::g_guiOpen || hasActiveCallback();
	script::onPresent();
	for (auto& c : m_callbacks)
		c.invoke();
}

LRESULT renderer::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (g_renderer.get()) {
		if (ImGui::GetCurrentContext())
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		shv::wndProc(hWnd, uMsg, wParam, lParam);
		if (uMsg == WM_KEYDOWN && wParam == VK_INSERT) {
			script::g_guiOpen ^= true;
		}
		return CallWindowProcA(g_renderer->m_wndProc, hWnd, uMsg, wParam, lParam);
	}
	return -1;
}

std::pair<shaderData, vec2> renderer::createTexture(fs::path path) {
	image image{ stb::file::readImage(path) };
	return std::make_pair(createShaderData(image), image.size);
}
shaderData renderer::createShaderData(image image) {
	IS_VALID(image.data);
	ID3D11ShaderResourceView* resourceView{};
	ID3D11Texture2D* texture{};
	D3D11_TEXTURE2D_DESC desc{ image.size.x.u32, image.size.y.u32, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, { 1 }, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0 };
	D3D11_SUBRESOURCE_DATA subResource{ image.data, desc.Width * 4, 0 };
	m_device->CreateTexture2D(&desc, &subResource, &texture);
	IS_VALID(texture);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{ DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_SRV_DIMENSION_TEXTURE2D, { 0, desc.MipLevels } };
	m_device->CreateShaderResourceView(texture, &srvDesc, std::add_pointer_t<decltype(resourceView)>(&resourceView));
	RETURN_IF_VALID(texture, resourceView);
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