#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "STB/STBImage.hpp"
#include "STB/STBImageWrite.hpp"
#include "Renderer.hpp"
#include "Pch.hpp"
#include "Draw.hpp"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace STB {
	namespace Memory {
		ImageData Write(Vec2& Size, int comp, const void* data, int strideBytes) {
			int ImageSize;
			stbi_uc* Image{ stbi_write_png_to_mem((const unsigned char*)data, strideBytes, Size.x.i32, Size.y.i32, comp, &ImageSize) };
			RETURN_IF_VALID(Image, ImageSize)
		}
		Image Read(ImageData data) {
			Vec2 Size{};
			stbi_uc* Image{ stbi_load_from_memory(data.Bytes, data.Size, &Size.x.i32, &Size.y.i32, NULL, 0) };
			RETURN_IF_VALID(Image, Size)
		}
	}
	namespace File {
		Image Read(fs::path path) {
			Vec2 Size{};
			stbi_uc* Image{ stbi_load(path.string().c_str(), &Size.x.i32, &Size.y.i32, NULL, 4) };
			RETURN_IF_VALID(Image, Size)
		}
	}
}

void CreateImFont(ImFontAtlas* Atlas, ImFont*& Font, fs::path Path, float Size, ImFontConfig* Config) {
	Font = Atlas->AddFontFromFileTTF(Path.string().c_str(), Size, Config);
}
template <size_t DataSize>
void CreateImFont(ImFontAtlas* Atlas, ImFont*& Font, uint8_t(&Data)[DataSize], float Size, ImFontConfig* Config) {
	Font = Atlas->AddFontFromMemoryTTF(Data, DataSize, Size, Config);
}

Renderer::Renderer() {
	m_WndClassEx = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandleA(0), 0, 0, 0, 0, BRAND, 0 };
	RegisterClassExA(&m_WndClassEx);
	m_Hwnd = CreateWindowExA(WS_EX_TRANSPARENT | WS_EX_LAYERED, m_WndClassEx.lpszClassName, BRAND, WS_DISABLED, 200, 200, 0, 0, NULL, NULL, m_WndClassEx.hInstance, NULL);
	if (!CreateD3D(m_Hwnd)) {
		CleanupD3D();
		UnregisterClassA(m_WndClassEx.lpszClassName, m_WndClassEx.hInstance);
	}
	SetLayeredWindowAttributes(m_Hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
	ShowWindow(m_Hwnd, SW_HIDE);
	UpdateWindow(m_Hwnd);
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui_ImplWin32_Init(m_Hwnd);
	ImGui_ImplDX11_Init(m_Device, m_Context);
	m_Config.FontDataOwnedByAtlas = false;
	fs::path Path{ fs::path("C:").append("Windows").append("Fonts") };
	fs::path ResourcePath{ fs::path(std::getenv("appdata")).append(BRAND).append("Resources") };
	ImFontAtlas* Atlas{ ImGui::GetIO().Fonts };
	CreateImFont(Atlas, m_Font, fs::path(Path).append("Arial.ttf"), 19.f, &m_Config);
	Draw::Init();
}
Renderer::~Renderer() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	CleanupD3D();
	DestroyWindow(m_Hwnd);
	UnregisterClassA(m_WndClassEx.lpszClassName, m_WndClassEx.hInstance);
}
constexpr float g_RenderTargetRGBA[4] = { 0.2f, 0.2f, 0.2f, 0.f };
bool Renderer::Present() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	Draw::Tick();
	ImGui::EndFrame();
	ImGui::Render();
	m_Context->OMSetRenderTargets(1, &m_TargetView, NULL);
	m_Context->ClearRenderTargetView(m_TargetView, g_RenderTargetRGBA);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::UpdatePlatformWindows(); //Called for the viewport
	ImGui::RenderPlatformWindowsDefault(); //Called for the viewport
	m_SwapChain->Present(1, 0); //Present with vsync
	return true;
}
LRESULT WINAPI Renderer::WndProc(HWND Hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(Hwnd, Msg, wParam, lParam))
		return true;
	if (Msg == WM_SIZE && g_Renderer.get() && g_Renderer->m_Device != NULL && wParam != SIZE_MINIMIZED) {
		g_Renderer->CleanupRenderTargets();
		g_Renderer->m_SwapChain->ResizeBuffers(0, UINT(LOWORD(lParam)), UINT(HIWORD(lParam)), DXGI_FORMAT_UNKNOWN, 0);
		g_Renderer->CreateRenderTargets();
		return 0;
	}
	else if (Msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(Hwnd, Msg, wParam, lParam);
}
bool Renderer::CreateD3D(HWND hWnd) {
	//Setup the sc
	DXGI_SWAP_CHAIN_DESC Desc = {
		{ 0, 0, { 60, 1 }, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED, DXGI_MODE_SCALING_UNSPECIFIED },
		{ 1, 0 },
		DXGI_USAGE_RENDER_TARGET_OUTPUT,
		2,
		hWnd,
		TRUE,
		DXGI_SWAP_EFFECT_DISCARD,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	};
	UINT DeviceCreationFlags{};
	D3D_FEATURE_LEVEL FeatureLevel{};
	const D3D_FEATURE_LEVEL FeatureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, DeviceCreationFlags, FeatureLevelArray, 2, D3D11_SDK_VERSION, &Desc, &m_SwapChain, &m_Device, &FeatureLevel, &m_Context) != S_OK)
		return false;
	CreateRenderTargets();
	return true;
}
void Renderer::CleanupD3D() {
	CleanupRenderTargets();
	if (m_SwapChain) {
		m_SwapChain->Release();
		m_SwapChain = nullptr;
	}
	if (m_BlendState) {
		m_BlendState->Release();
		m_BlendState = nullptr;
	}
	if (m_Device) {
		m_Device->Release();
		m_Device = nullptr;
	}
	if (m_Context) {
		m_Context->Release();
		m_Context = nullptr;
	}
}
void Renderer::CreateRenderTargets() {
	ID3D11Texture2D* BackBuffer;
	m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&BackBuffer));
	m_Device->CreateRenderTargetView(BackBuffer, NULL, &m_TargetView);
	BackBuffer->Release();
}
void Renderer::CleanupRenderTargets() {
	if (m_TargetView) {
		m_TargetView->Release();
		m_TargetView = NULL;
	}
}
std::pair<ShaderData, Vec2> Renderer::CreateTexture(fs::path path) {
	Image Image{ STB::File::Read(path) };
	return std::make_pair(CreateShaderData(Image), Image.Size);
}
ShaderData Renderer::CreateShaderData(Image Image) {
	IS_VALID(Image.Data);
	ID3D11ShaderResourceView* ResourceView{};
	ID3D11Texture2D* Texture{};
	D3D11_TEXTURE2D_DESC Desc{ Image.Size.x.u32, Image.Size.y.u32, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, { 1 }, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0 };
	D3D11_SUBRESOURCE_DATA SubResource{ Image.Data, Desc.Width * 4, 0 };
	m_Device->CreateTexture2D(&Desc, &SubResource, &Texture);
	IS_VALID(Texture);
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc{ DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_SRV_DIMENSION_TEXTURE2D, { 0, Desc.MipLevels } };
	m_Device->CreateShaderResourceView(Texture, &ShaderResourceViewDesc, std::add_pointer_t<decltype(ResourceView)>(&ResourceView));
	RETURN_IF_VALID(Texture, ResourceView);
}