#include "hooking/hooking.h"
#include "renderer/renderer.h"
#include "commands/manager/manager.h"

i64 hooks::updateTimecycleData(u64* _This, TimecycleKeyframeData* pData) {
	i64 ret{ CALL(updateTimecycleData, _This, pData) };
	if ("azimuthEastColor"_TCC->get(0).toggle) {
		pData->m_azimuth_east_color.setValueRGBA("azimuthEastColor"_TCC->get_color().float4());
	}
	if ("azimuthEastIntensity"_TFC->get(0).toggle) {
		pData->m_azimuth_east_intensity = "azimuthEastIntensity"_TFC->get(1).floating_point;
	}
	if ("azimuthWestColor"_TCC->get(0).toggle) {
		pData->m_azimuth_west_color.setValueRGBA("azimuthWestColor"_TCC->get_color().float4());
	}
	if ("azimuthWestIntensity"_TFC->get(0).toggle) {
		pData->m_azimuth_west_intensity = "azimuthWestIntensity"_TFC->get(1).floating_point;
	}
	if ("azimuthTransitionColor"_TCC->get(0).toggle) {
		pData->m_azimuth_transition_color.setValueRGBA("azimuthTransitionColor"_TCC->get_color().float4());
	}
	if ("azimuthTransitionIntensity"_TFC->get(0).toggle) {
		pData->m_azimuth_transition_intensity = "azimuthTransitionIntensity"_TFC->get(1).floating_point;
	}
	if ("azimuthTransitionPosition"_TFC->get(0).toggle) {
		pData->m_azimuth_transition_position.m_value = "azimuthTransitionPosition"_TFC->get(1).floating_point;
	}
	if ("zenithColor"_TCC->get(0).toggle) {
		pData->m_zenith_color.setValueRGBA("zenithColor"_TCC->get_color().float4());
	}
	if ("zenithIntensity"_TFC->get(0).toggle) {
		pData->m_zenith_intensity = "zenithIntensity"_TFC->get(1).floating_point;
	}
	if ("zenithTransitionColor"_TCC->get(0).toggle) {
		pData->m_zenith_transition_color.setValueRGBA("zenithTransitionColor"_TCC->get_color().float4());
	}
	if ("zenithConstantColors"_TCC->get(0).toggle) {
		pData->m_zenith_constants.setValueRGBA("zenithConstantColors"_TCC->get_color().float4());
	}
	if ("skyPlaneColor"_TCC->get(0).toggle) {
		pData->m_sky_plane_color.setValueRGBA("skyPlaneColor"_TCC->get_color().float4());
	}
	if ("skyPlaneParams"_TIC->get(0).toggle) {
		pData->m_sky_plane_params.m_index = "skyPlaneParams"_TIC->get(1).i32;
	}
	if ("sunDirection"_TIC->get(0).toggle) {
		pData->m_sun_direction.m_index = "sunDirection"_TIC->get(1).i32;
	}
	if ("sunPosition"_TIC->get(0).toggle) {
		pData->m_sun_position.m_index = "sunPosition"_TIC->get(1).i32;
	}
	if ("sunColor"_TCC->get(0).toggle) {
		pData->m_sun_color.setValueRGBA("sunColor"_TCC->get_color().float4());
	}
	if ("sunColorHdr"_TCC->get(0).toggle) {
		pData->m_sun_color_hdr.setValueRGBA("sunColorHdr"_TCC->get_color().float4());
	}
	if ("sunDiscColorHdr"_TCC->get(0).toggle) {
		pData->m_sun_disc_color_hdr.setValueRGBA("sunDiscColorHdr"_TCC->get_color().float4());
	}
	if ("sunConstantColors"_TCC->get(0).toggle) {
		pData->m_sun_constants.setValueRGBA("sunConstantColors"_TCC->get_color().float4());
	}
	if ("cloudConstant1Color"_TCC->get(0).toggle) {
		pData->m_cloud_constant1.setValueRGBA("cloudConstant1Color"_TCC->get_color().float4());
	}
	if ("cloudConstant2Color"_TCC->get(0).toggle) {
		pData->m_cloud_constant2.setValueRGBA("cloudConstant2Color"_TCC->get_color().float4());
	}
	if ("cloudConstant3Color"_TCC->get(0).toggle) {
		pData->m_cloud_constant3.setValueRGBA("cloudConstant3Color"_TCC->get_color().float4());
	}
	if ("cloudDetailConstantColor"_TCC->get(0).toggle) {
		pData->m_cloud_detail_constants.setValueRGBA("cloudDetailConstantColor"_TCC->get_color().float4());
	}
	if ("cloudBaseColorMinusMidColor"_TCC->get(0).toggle) {
		pData->m_cloud_base_minus_mid_colour.setValueRGBA("cloudBaseColorMinusMidColor"_TCC->get_color().float4());
	}
	if ("cloudMidColor"_TCC->get(0).toggle) {
		pData->m_cloud_mid_color.setValueRGBA("cloudMidColor"_TCC->get_color().float4());
	}
	if ("cloudShadowColorMinusBaseColorTimesShadowStrength"_TCC->get(0).toggle) {
		pData->m_cloud_shadow_minus_base_color_times_shadow_strength.setValueRGBA("cloudShadowColorMinusBaseColorTimesShadowStrength"_TCC->get_color().float4());
	}
	if ("smallCloudConstantColors"_TCC->get(0).toggle) {
		pData->m_small_cloud_constants.setValueRGBA("smallCloudConstantColors"_TCC->get_color().float4());
	}
	if ("smallCloudColorHdr"_TCC->get(0).toggle) {
		pData->m_small_cloud_color_hdr.setValueRGBA("smallCloudColorHdr"_TCC->get_color().float4());
	}
	if ("cloudGenerationFrequency"_TIC->get(0).toggle) {
		pData->m_cloudgen_frequency.m_index = "cloudGenerationFrequency"_TIC->get(1).i32;
	}
	if ("noisePhaseColor"_TCC->get(0).toggle) {
		pData->m_noise_phase.setValueRGBA("noisePhaseColor"_TCC->get_color().float4());
	}
	if ("speedConstants"_TIC->get(0).toggle) {
		pData->m_speed_constants.m_index = "speedConstants"_TIC->get(1).i32;
	}
	if ("horizonLevel"_TFC->get(0).toggle) {
		pData->m_horizon_level.m_value = "horizonLevel"_TFC->get(1).floating_point;
	}
	if ("starfieldIntensity"_TFC->get(0).toggle) {
		pData->m_starfield_intensity.m_value = "starfieldIntensity"_TFC->get(1).floating_point;
	}
	if ("moonIntensity"_TFC->get(0).toggle) {
		pData->m_moon_intensity.m_value = "moonIntensity"_TFC->get(1).floating_point;
	}
	if ("moonColor"_TCC->get(0).toggle) {
		pData->m_moon_color.setValueRGBA("moonColor"_TCC->get_color().float4());
	}
	if ("lunarCycle"_TIC->get(0).toggle) {
		pData->m_lunar_cycle.m_index = "lunarCycle"_TIC->get(1).i32;
	}
	if ("moonDirection"_TIC->get(0).toggle) {
		pData->m_moon_direction.m_index = "moonDirection"_TIC->get(1).i32;
	}
	if ("moonPosition"_TIC->get(0).toggle) {
		pData->m_moon_position.m_index = "moonPosition"_TIC->get(1).i32;
	}
	if ("noiseFrequency"_TFC->get(0).toggle) {
		pData->m_noise_frequency.m_value = "noiseFrequency"_TFC->get(1).floating_point;
	}
	if ("noiseScale"_TFC->get(0).toggle) {
		pData->m_noise_scale.m_value = "noiseScale"_TFC->get(1).floating_point;
	}
	if ("noiseThreshold"_TFC->get(0).toggle) {
		pData->m_noise_threshold.m_value = "noiseThreshold"_TFC->get(1).floating_point;
	}
	if ("noiseSoftness"_TFC->get(0).toggle) {
		pData->m_noise_softness.m_value = "noiseSoftness"_TFC->get(1).floating_point;
	}
	if ("noiseDensityOffset"_TFC->get(0).toggle) {
		pData->m_noise_density_offset.m_value = "noiseDensityOffset"_TFC->get(1).floating_point;
	}
	pData->m_update_sky_attributes = true;
	//what in the blursed
	static auto txt{ g_renderer->createTexture(fs::path(std::getenv("appdata")).append(BRAND).append("Textures").append("Sky.png")) };
	static auto copy{ pData->m_star_field.m_texture->CreateCopy() };
	if (!g_running) {
		pData->m_star_field.setShader(copy.m_shader, copy.m_resource);
	}
	else {
		pData->m_star_field.setShader(txt.first.resourceView, txt.first.resource);
	}
	return ret;
}