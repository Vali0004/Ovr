#pragma once
#include "Pch.hpp"
#define RETURN(...) return { __VA_ARGS__ };
#define RETURN_DEFAULT return {};
#define IS_VALID(Check) \
	if (!Check) { \
		RETURN_DEFAULT; \
	}
#define RETURN_IF_VALID(Check, ...) IS_VALID(Check) RETURN(Check, __VA_ARGS__)

union VectorCoord {
	int16_t i16;
	uint16_t u16;
	int32_t i32;
	uint32_t u32;
	int32_t i64;
	uint32_t u64;
	float fPoint;
};
class Vec2 {
public:
	VectorCoord x{}, y{};
};
class Image {
public:
	unsigned char* Data;
	Vec2 Size;
};
class ImageData {
public:
	unsigned char* Bytes;
	int Size;
};
class ShaderData {
public:
	ID3D11Resource* Resource{};
	ID3D11ShaderResourceView* ResourceView{};
};
class Resource {
public:
	Resource(std::string Name, ShaderData Data) : m_Name(Name), m_Data(Data) {}
	Resource() : Resource({}, {}) {}
	ID3D11ShaderResourceView* GetShaderResource() {
		return m_Data.ResourceView;
	}
	bool Matches(const char* ptr) {
		if (!m_Name.compare(ptr)) {
			return true;
		}
		return false;
	}
private:
	std::string m_Name{};
	ShaderData m_Data{};
};
class Resources {
public:
	Resource Get(std::string key) {
		if (this) {
			for (auto& r : m_ResourceTable) {
				if (r.Matches(key.c_str())) {
					return r;
				}
			}
		}
		return {};
	}
	void Add(Resource resource) {
		m_ResourceTable.push_back(resource);
	}
	ID3D11ShaderResourceView* operator[](std::string key) {
		return Get(key).GetShaderResource();
	}
private:
	std::vector<Resource> m_ResourceTable{};
};
class Renderer {
public:
	Renderer();
	~Renderer();
public:
	bool CreateD3D(HWND hWnd);
	void CleanupD3D();
	void CreateRenderTargets();
	void CleanupRenderTargets();
	bool Present();
	static LRESULT WINAPI WndProc(HWND Hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	std::pair<ShaderData, Vec2> CreateTexture(fs::path Path);
	ShaderData CreateShaderData(Image Image);
public:
	Resources m_Resources{};
public:
	ImFontConfig m_Config{};
	ImFont* m_Font{};
private:
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_Context;
	IDXGISwapChain* m_SwapChain;
	ID3D11RenderTargetView* m_TargetView;
	ID3D11BlendState* m_BlendState;
	DXGI_SWAP_CHAIN_DESC m_Desc;
	HWND m_Hwnd;
	WNDCLASSEX m_WndClassEx;
	ImVec2 m_WindowSize{ 200, 200 };
};
inline std::unique_ptr<Renderer> g_Renderer;