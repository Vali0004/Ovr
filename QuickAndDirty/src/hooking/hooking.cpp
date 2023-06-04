#include "hooking.h"
#include "memory/pointers.h"

hooking::hooking() :
	m_minhook(),
	m_cTaskJumpConstructor("CTJC", pointers::g_cTaskJumpConstructor, &hooks::cTaskJumpConstructor),
	m_cTaskFallConstructor("CTFC", pointers::g_cTaskFallConstructor, &hooks::cTaskFallConstructor),
	m_runAsyncModuleRequest("RAMS", pointers::g_runAsyncModuleRequest, &hooks::runAsyncModuleRequest),
	m_hasIntervalElapsed("HIE", pointers::g_hasIntervalElapsed, &hooks::hasIntervalElapsed),
	m_gameSkeletonRunUpdate("GSRU", pointers::g_gameSkeletonRunUpdate, &hooks::gameSkeletonRunUpdate),
	m_gameSkeletonUpdateBaseRunGroup("GSUBRG", pointers::g_gameSkeletonUpdateBaseRunGroup, &hooks::gameSkeletonUpdateBaseRunGroup),
	m_dispatchEvent("DE", pointers::g_dispatchEvent, &hooks::dispatchEvent),
	m_convertThreadToFiber("CTTF", *dynamicFunctionParser{ "kernel32.dll", "ConvertThreadToFiber" }, &hooks::convertThreadToFiber),
	m_getProcAddress("GPA", &GetProcAddress, &hooks::getProcAddress),
	m_DX(
		VFT({ *pointers::g_swapChain, g_swapchainSize }),
		VFT({ VFTFunctionData(&hooks::present, g_presentIndex), VFTFunctionData(&hooks::resizeBuffers, g_resizeBuffersIndex) })
	) {
}
hooking::~hooking() {
}

void hooking::enable() {
	m_DX.enable();
	detour::enableAll();
	MH_ApplyQueued();
}
void hooking::disable() {
	//Sometimes the pointer is deallocated, but disabled wasn't called.
	// We'll just do a MinHook call and have the OS cleanup our mess
	if (this) {
		m_DX.disable();
	}
	detour::disableAll();
	MH_ApplyQueued();
}