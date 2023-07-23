#include "hooking/hooking.h"
#include "renderer/renderer.h"

i64 hooks::updateTimecycleData(u64* _This, TimecycleKeyframeData* pData) {
	i64 ret{ CALL(updateTimecycleData, _This, pData) };
	//pData->m_azimuth_east_color.m_value = { 1.f, 0.f, 0.f, 0.f };
	//pData->m_azimuth_east_intensity = 0.f;
	//pData->m_azimuth_west_color.m_value = { 1.f, 0.f, 0.f, 0.f };
	//pData->m_azimuth_west_intensity = 0.f;
	////m_sun_color.m_value.x == r, m_sun_color.m_value.y == 
	//pData->m_sun_color.m_value = { 1.f, 0.f, 0.f, 0.f };
	//pData->m_update_sky_attributes = true;
	//////what in the blursed
	////static auto txt{ g_renderer->createTexture(fs::path(std::getenv("appdata")).append("Ovr").append("Textures").append("Sky.png")) };
	////pData->m_star_field.m_shader->m_shader = txt.first.resourceView;
	////pData->m_star_field.m_shader->m_texture = txt.first.resource;
	return ret;
}