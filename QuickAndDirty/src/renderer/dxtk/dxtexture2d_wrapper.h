#pragma once
#include "pch/pch.h"

using namespace DirectX;
class dxTexture2DWrapper {
public:
	dxTexture2DWrapper() :
		m_desc(DXGI_FORMAT_UNKNOWN, 256, 256, 1, 0, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, D3D11_USAGE_DEFAULT, 0, 1, 0, D3D11_RESOURCE_MISC_GENERATE_MIPS)
	{
	}
	float imgScale(ImVec2 size, ImVec2 scale) {
		float ret{ 1.f };
		if (size.x > size.y)
			ret = size.x * scale.x;
		if (size.y > size.x)
			ret = size.y * scale.y;
		return ret;
	}
	void draw(SpriteBatch& batch) {
		ImVec2 position{ m_pos * ImGui::GetIO().DisplaySize };
		XMFLOAT2 xmPos{ position.x, position.y };
		XMVECTOR xmCol{ m_color.x, m_color.y, m_color.z, m_color.w };
		XMFLOAT2 origin{ m_center.x * m_desc.Width, m_center.x * m_desc.Height };
		float scale{ imgScale(ImGui::GetIO().DisplaySize / m_desc.Width, m_size) };
		float rot{ m_rotation * 2 * static_cast<float>(PI) };
		batch.Draw(m_view.Get(), xmPos, nullptr, xmCol, rot, origin, scale);
		if (GetTickCount64() > m_disableTime)
			m_enabled = false;
	}
	void setProperties(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a) {
		m_enabled = true;
		m_id = id;
		m_index = index;
		m_level = level;
		m_time = time;
		m_size.x = sizeX;
		m_size.y = sizeY;
		m_center.x = centerX;
		m_center.y = centerY;
		m_pos.x = posX;
		m_pos.y = posY;
		m_rotation = rotation;
		m_screenHeightScaleFactor = screenHeightScaleFactor;
		m_color.x = r;
		m_color.y = g;
		m_color.z = b;
		m_color.w = a;
		m_disableTime = GetTickCount64() + time;
	}
private:
	int m_id{};
	int m_index{};
	int m_level{};
	int m_time{};
	ImVec2 m_size{};
	ImVec2 m_center{};
	ImVec2 m_pos{};
	float m_rotation;
	float m_screenHeightScaleFactor;
	ImVec4 m_color{};
public:
	bool m_enabled{};
	DWORD64 m_disableTime{};
	comPtr<ID3D11ShaderResourceView> m_view{};
	comPtr<ID3D11Resource> m_resource{};
	comPtr<ID3D11Texture2D> m_texture{};
	CD3D11_TEXTURE2D_DESC m_desc{};
};
struct scriptTex {
	dxTexture2DWrapper tex;
	int id;
};
struct reloadTex {
	int id;
	std::wstring name;
};