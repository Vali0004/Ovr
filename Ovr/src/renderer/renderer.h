#pragma once
#include "pch/pch.h"
#include "memory/pointers.h"
#include "rage/commands/list.h"
#include "renderer/dxtk/state_saver.h"

class renderer {
public:
	renderer();
	~renderer();
public:
	void onPresent();
	static LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	//SHV fuckery
	void createTextures();
	void reloadTextures();
	int createTexture(const char* texFileName);
	void drawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a);
public:
	ImFont* m_font{};
	ImFont* m_tahoma{};
	ImFont* m_arial{};
	ImFontConfig m_fontCfg{};
public:
	bool m_stateSaved{};
	std::unique_ptr<stateSaver> m_stateSaver{};
	std::unique_ptr<DirectX::CommonStates> m_commonState{};
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch{};
	comPtr<IDXGISwapChain> m_swapchain{};
	comPtr<ID3D11DeviceContext> m_context{};
private:
	WNDPROC m_wndProc{};
	comPtr<ID3D11Device> m_device{};
};
inline std::unique_ptr<renderer> g_renderer{};