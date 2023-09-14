#include "hooking.h"
#include "memory/pointers.h"
#include "hooking/methods/native.h"
#include "commands/features.h"
#include "util/game_vm_guard.h"
#include "util/transaction.h"

hooking::hooking() :
	m_minhook(),
	m_cTaskJumpConstructor("CTJC", pointers::g_cTaskJumpConstructor, &hooks::cTaskJumpConstructor),
	m_cTaskFallConstructor("CTFC", pointers::g_cTaskFallConstructor, &hooks::cTaskFallConstructor),
	m_runAsyncModuleRequest("RAMR", pointers::g_runAsyncModuleRequest, &hooks::runAsyncModuleRequest),
	m_dispatchEvent("DE", pointers::g_dispatchEvent, &hooks::dispatchEvent),
	m_scriptVm("SV", pointers::g_scriptVm, &hooks::scriptVm),
	m_proccessPackedEvents("PPE", pointers::g_proccessPackedEvents, &hooks::proccessPackedEvents),
	m_receiveCloneSync("RCS", pointers::g_receiveCloneSync, &hooks::receiveCloneSync),
	m_receiveCloneCreate("RCC", pointers::g_receiveCloneCreate, &hooks::receiveCloneCreate),
	m_canApplyData("CAD", pointers::g_canApplyData, &hooks::canApplyData),
	m_findGameMatch("FGM", pointers::g_findGameMatch, &hooks::findGameMatch, false),
	m_processMatchmakingFind("PMF", pointers::g_processMatchmakingFind, &hooks::processMatchmakingFind, false),
	m_calculateMipLevel("CML", pointers::g_calculateMipLevel, &hooks::calculateMipLevel),
	m_hasRosPrivilege("HRP", pointers::g_hasRosPrivilege, &hooks::hasRosPrivilege),
	m_insertStreamingModule("ISM", pointers::g_insertStreamingModule, &hooks::insertStreamingModule),
	m_updateTimecycleData("TCD", pointers::g_updateTimecycleData, &hooks::updateTimecycleData),
	m_allocateReliable("AR", pointers::g_allocateReliable, &hooks::allocateReliable),
	m_writePlayerGameStateDataNode("WPGSDN", pointers::g_writePlayerGameStateDataNode, &hooks::writePlayerGameStateDataNode, false),
	m_getNewsStory("GNS", pointers::g_getNewsStory, &hooks::getNewsStory),
	m_presencePostRawMessage("RPM", pointers::g_presencePostRawMessage, &hooks::presencePostRawMessage),
	m_getAvailableMemoryForStreamer("GAMFS", pointers::g_getAvailableMemoryForStreamer, &hooks::getAvailableMemoryForStreamer),
	m_settingsVramTex("SVT", pointers::g_settingsVramTex, &hooks::settingsVramTex),
	m_hasGameBeenAltered("HGBA", pointers::g_hasGameBeenAltered, &hooks::hasGameBeenAltered),
	m_addItemToBasket("AITB", pointers::g_addItemToBasket, &hooks::addItemToBasket),
	m_request("R", pointers::g_request, &hooks::request),
	m_sendMetric("SM", pointers::g_sendMetric, &hooks::sendMetric),
	m_sendNetworkEvent("SNE", pointers::g_sendNetworkEvent, &hooks::sendNetworkEvent),
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
	createNativeShim("all_scripts"_joaat, 0x580CE4438479CC61, [](rage::scrThread::Info* info) {
		info->ResultPtr->Int = FALSE;
	});
	createNativeShim("all_scripts"_joaat, 0x95914459A87EBA28, [](rage::scrThread::Info* info) {
		info->ResultPtr->Int = FALSE;
	});
	createNativeShim("all_scripts"_joaat, 0xEAA572036990CD1B, [](rage::scrThread::Info* info) {
		info->ResultPtr->Int = FALSE;
	});
	createNativeShim("all_scripts"_joaat, 0x158C16F5E4CF41F8, [](rage::scrThread::Info* info) {
		info->ResultPtr->Int = TRUE;
	});

	UnhookWindowsHookEx(*pointers::g_windowHook.add(44).call().as<HHOOK*>());
	m_DX.enable();
	detour::enableAll();
	MH_ApplyQueued();
	budgeting::postInit();
}
void hooking::disable() {
	SetWindowsHookExA(WH_KEYBOARD_LL, *pointers::g_windowHook.add(17).call().as<HOOKPROC>(), GetModuleHandleA(NULL), 0);
	//Sometimes the g_hooking pointer is deallocated, but disabled wasn't called.
	// We'll just do a MinHook call and have the OS cleanup our mess
	if (this) {
		m_DX.disable();
	}
	detour::disableAll();
	MH_ApplyQueued();
	resetNativeShims();
}