#include "hooking/hooking.h"
#include "renderer/renderer.h"

i64 hooks::updateTimecycleData(u64* _This, TimecycleKeyframeData* pData) {
	i64 ret{ CALL(updateTimecycleData, _This, pData) };
	pData->m_azimuth_east_color.setValueRGBA(255, 0, 255, 255);
	pData->m_azimuth_west_color.setValueRGBA(10, 0, 255, 255);
	pData->m_sun_color.setValueRGBA(255, 0, 0, 255);
	pData->m_update_sky_attributes = true;
	//what in the blursed
	static auto txt{ g_renderer->createTexture(fs::path(std::getenv("appdata")).append("Ovr").append("Textures").append("Sky.png")) };
	static auto copy{ pData->m_star_field.m_texture->CreateCopy() };
	if (!g_running) {
		pData->m_star_field.setShader(copy.m_shader, copy.m_resource);
	}
	else {
		pData->m_star_field.setShader(txt.first.resourceView, txt.first.resource);
	}
	return ret;
}