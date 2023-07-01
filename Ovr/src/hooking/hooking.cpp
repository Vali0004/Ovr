#include "hooking.h"
#include "memory/pointers.h"
#include "hooking/methods/native.h"

hooking::hooking() :
	m_minhook(),
	m_cTaskJumpConstructor("CTJC", pointers::g_cTaskJumpConstructor, &hooks::cTaskJumpConstructor),
	m_cTaskFallConstructor("CTFC", pointers::g_cTaskFallConstructor, &hooks::cTaskFallConstructor, false),
	m_runAsyncModuleRequest("RAMS", pointers::g_runAsyncModuleRequest, &hooks::runAsyncModuleRequest),
	m_hasIntervalElapsed("HIE", pointers::g_hasIntervalElapsed, &hooks::hasIntervalElapsed),
	m_dispatchEvent("DE", pointers::g_dispatchEvent, &hooks::dispatchEvent),
	m_scriptVm("SV", pointers::g_scriptVm, &hooks::scriptVm),
	m_proccessPackedEvents("PPE", pointers::g_proccessPackedEvents, &hooks::proccessPackedEvents),
	m_receiveCloneSync("RCS", pointers::g_receiveCloneSync, &hooks::receiveCloneSync),
	m_receiveCloneCreate("RCC", pointers::g_receiveCloneCreate, &hooks::receiveCloneCreate),
	m_canApplyData("CAD", pointers::g_canApplyData, &hooks::canApplyData),
	m_findGameMatch("FGM", pointers::g_findGameMatch, &hooks::findGameMatch, false),
	m_processMatchmakingFind("PMF", pointers::g_processMatchmakingFind, &hooks::processMatchmakingFind, false),
	m_addItemToBasket("AITB", pointers::g_addItemToBasket, &hooks::addItemToBasket),
	m_request("R", pointers::g_request, &hooks::request),
	m_sendMetric("SM", pointers::g_sendMetric, &hooks::sendMetric),
	m_prepareMetric("SM", pointers::g_prepareMetric, &hooks::prepareMetric),
	m_sendNetworkEvent("SNE", pointers::g_sendNetworkEvent, &hooks::sendNetworkEvent),
	m_addEventToList("AETL", pointers::g_addEventToList, &hooks::addEventToList),
	m_updateAttributeInt("SV", pointers::g_presenceData[g_updateAttributeIntIndex], &hooks::updateAttributeInt),
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