#pragma once
#include "pch/pch.h"
#include "memory/pointers.h"
#include "rage/commands/list.h"
#include "renderer/dxtk/state_saver.h"
#define RETURN(...) return { __VA_ARGS__ };
#define RETURN_DEFAULT return {};
#define IS_VALID(check) \
	if (!check) { \
		RETURN_DEFAULT; \
	}
#define RETURN_IF_VALID(check, ...) IS_VALID(check) RETURN(check, __VA_ARGS__)

class callback {
public:
	callback(bool active, std::function<void(bool&)> fn) : m_active(active), m_fn(fn) {}
	callback(std::function<void(bool&)> fn) : callback(true, fn) {}
public:
	void invoke() {
		if (m_fn) {
			if (m_active) {
				m_fn(m_active);
			}
		}
	}
	operator bool() {
		return m_active;
	}
private:
	bool m_active{};
	std::function<void(bool&)> m_fn{};
};
struct vec2 {
	union vectorCordValue {
		int16_t i16;
		uint16_t u16;
		int32_t i32;
		uint32_t u32;
		int32_t i64;
		uint32_t u64;
		float fPoint;
	};
	vectorCordValue x{}, y{};
};
struct image { unsigned char* data; vec2 size; };
struct imageData { unsigned char* bytes; int size; uint32_t delay; };
struct shaderData { ID3D11Resource* resource{}; ID3D11ShaderResourceView* resourceView{}; };
namespace stb {
	namespace memory {
		extern imageData write(vec2& size, int comp, const void* data, int strideBytes, uint32_t delay);
		extern image read(imageData data);
	}
	namespace file {
		extern image readImage(fs::path path);
	}
}
class renderer {
public:
	renderer();
	~renderer();

	void onPresent();
	static void onTick();
	static LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	std::pair<shaderData, vec2> createTexture(fs::path path);
	shaderData createShaderData(image image);

	//SHV fuckery
	void createTextures();
	void reloadTextures();
	int createTexture(const char* texFileName);
	void drawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a);

	ImFont* m_font{};
	ImFont* m_fontBold{};
	ImFont* m_tahoma{};
	ImFont* m_arial{};
	ImFontConfig m_fontCfg{};

	bool hasActiveCallback() {
		for (auto& c : m_callbacks) {
			if (c) {
				return true;
			}
		}
		return false;
	}
	std::vector<callback> m_callbacks{};
	bool m_stateSaved{};
	SmartPointer<stateSaver> m_stateSaver{};
	SmartPointer<DirectX::CommonStates> m_commonState{};
	SmartPointer<DirectX::SpriteBatch> m_spriteBatch{};
	comPtr<IDXGISwapChain> m_swapchain{};
	comPtr<ID3D11DeviceContext> m_context{};
private:
	WNDPROC m_wndProc{};
	comPtr<ID3D11Device> m_device{};
};
inline SmartPointer<renderer> g_renderer{};