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
	m_runAsyncModuleRequest("RAMS", pointers::g_runAsyncModuleRequest, &hooks::runAsyncModuleRequest),
	m_dispatchEvent("DE", pointers::g_dispatchEvent, &hooks::dispatchEvent),
	m_scriptVm("SV", pointers::g_scriptVm, &hooks::scriptVm, false),
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
	m_writePlayerGameStateDataNode("WPGSDN", pointers::g_writePlayerGameStateDataNode, &hooks::writePlayerGameStateDataNode),
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

	//Has fucked coords, needs fixing (seamless transition)
	/*createNativeShim("all_scripts"_joaat, 0x40EB1EFD921822BC, [](rage::scrThread::Info* info) {});
	createNativeShim("all_scripts"_joaat, 0x340A36A700E99699, [](rage::scrThread::Info* info) {});
	createNativeShim("all_scripts"_joaat, 0x8E580AB902917360, [](rage::scrThread::Info* info) {});*/
	createNativeShim("maintransition"_joaat, 0x6BFB12CE158E3DD4, [](rage::scrThread::Info* info) { //SC_TRANSITION_NEWS_SHOW
		if ("seamlessTransition"_TC->get(0).toggle) {
			info->ResultPtr->Int = FALSE;
		}
		else {
			info->ResultPtr->Int = SOCIALCLUB::SC_TRANSITION_NEWS_SHOW(info->Params[0].Any);
		}
	});
	createNativeShim("maintransition"_joaat, 0xFE4C1D0D3B9CC17E, [](rage::scrThread::Info* info) { //SC_TRANSITION_NEWS_SHOW_TIMED
		if ("seamlessTransition"_TC->get(0).toggle) {
			info->ResultPtr->Int = FALSE;
		}
		else {
			info->ResultPtr->Int = SOCIALCLUB::SC_TRANSITION_NEWS_SHOW_TIMED(info->Params[0].Any, info->Params[1].Any);
		}
	});
	createNativeShim("maintransition"_joaat, 0x933BBEEB8C61B5F4, [](rage::scrThread::Info* info) { //IS_SWITCH_TO_MULTI_FIRSTPART_FINISHED
		if ("seamlessTransition"_TC->get(0).toggle) {
			info->ResultPtr->Int = TRUE;
		}
		else {
			info->ResultPtr->Int = STREAMING::IS_SWITCH_TO_MULTI_FIRSTPART_FINISHED();
		}
	});
	createNativeShim("maintransition"_joaat, 0x198F77705FA0931D, [](rage::scrThread::Info* info) { //SET_FOCUS_ENTITY
		if ("seamlessTransition"_TC->get(0).toggle) {
			STREAMING::SET_FOCUS_ENTITY(PLAYER::PLAYER_PED_ID());
		}
		else {
			STREAMING::SET_FOCUS_ENTITY(info->Params[0].Int);
		}
	});
	createNativeShim("maintransition"_joaat, 0x719FF505F097FD20, [](rage::scrThread::Info* info) { //HIDE_HUD_AND_RADAR_THIS_FRAME
		if (!"seamlessTransition"_TC->get(0).toggle) {
			HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();
		}
	});
	createNativeShim("maintransition"_joaat, 0xEF01D36B9C9D0C7B, [](rage::scrThread::Info* info) { //ACTIVATE_FRONTEND_MENU
		Hash menuhash{ info->Params[0].Uns };
		if ("seamlessTransition"_TC->get(0).toggle) {
			if (menuhash != "FE_MENU_VERSION_EMPTY_NO_BACKGROUND"_joaat) {
				HUD::ACTIVATE_FRONTEND_MENU(menuhash, info->Params[1].Int, info->Params[2].Int);
			}
		}
		else {
			HUD::ACTIVATE_FRONTEND_MENU(menuhash, info->Params[1].Int, info->Params[2].Int);
		}
	});
	createNativeShim("maintransition"_joaat, 0x10706DC6AD2D49C0, [](rage::scrThread::Info* info) { //RESTART_FRONTEND_MENU
		Hash menuhash{ info->Params[0].Uns };
		if ("seamlessTransition"_TC->get(0).toggle) {
			if (menuhash != "FE_MENU_VERSION_EMPTY_NO_BACKGROUND"_joaat) {
				HUD::RESTART_FRONTEND_MENU(menuhash, info->Params[1].Int);
			}
		}
		else {
			HUD::RESTART_FRONTEND_MENU(menuhash, info->Params[1].Int);
		}
	});
	createNativeShim("maintransition"_joaat, 0xDFC252D8A3E15AB7, [](rage::scrThread::Info* info) { //TOGGLE_PAUSED_RENDERPHASES
		if ("seamlessTransition"_TC->get(0).toggle) {
			GRAPHICS::RESET_PAUSED_RENDERPHASES();
		}
		else {
			GRAPHICS::TOGGLE_PAUSED_RENDERPHASES(info->Params[0].Int);
		}
	});
	createNativeShim("maintransition"_joaat, 0xEA1C610A04DB6BBB, [](rage::scrThread::Info* info) { //SET_ENTITY_VISIBLE
		Entity entity{ info->Params[0].Int };
		if ("seamlessTransition"_TC->get(0).toggle && entity == PLAYER::PLAYER_PED_ID()) {
			ENTITY::SET_ENTITY_VISIBLE(entity, TRUE, FALSE);
		}
		else {
			ENTITY::SET_ENTITY_VISIBLE(entity, info->Params[1].Int, info->Params[2].Int);
		}
	});
	//Currently has a logic error causing the coords to be at 0, 0, 1
	createNativeShim("maintransition"_joaat, 0x06843DA7060A026B, [](rage::scrThread::Info* info) { //SET_ENTITY_COORDS_NO_OFFSET
		Entity entity{ info->Params[0].Int };
		Vector3 pos_{ info->Params[1].Float, info->Params[2].Float, info->Params[3].Float };
		BOOL xAxis{ info->Params[4].Int };
		BOOL yAxis{ info->Params[5].Int };
		BOOL zAxis{ info->Params[6].Int };
		eTransitionState state{ static_cast<eTransitionState>(global(1574993).value()->Int) };
		if (!"seamlessTransition"_TC->get(0).toggle || state == eTransitionState::ConfirmFreemodeSessionJoining || entity != PLAYER::PLAYER_PED_ID()) {
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, pos_, xAxis, yAxis, zAxis);
		}
	});
	createNativeShim("maintransition"_joaat, 0x1A9205C1B9EE827F, [](rage::scrThread::Info* info) { //SET_ENTITY_COLLISION
		Entity entity{ info->Params[0].Int };
		if (!"seamlessTransition"_TC->get(0).toggle || entity != PLAYER::PLAYER_PED_ID()) {
			ENTITY::SET_ENTITY_COLLISION(entity, info->Params[1].Int, info->Params[2].Int);
		}
	});
	createNativeShim("maintransition"_joaat, 0x8D32347D6D4C40A2, [](rage::scrThread::Info* info) { //SET_PLAYER_CONTROL
		if (!"seamlessTransition"_TC->get(0).toggle) {
			PLAYER::SET_PLAYER_CONTROL(info->Params[0].Int, info->Params[1].Int, info->Params[2].Int);
		}
	});
	createNativeShim("maintransition"_joaat, 0x428CA6DBD1094446, [](rage::scrThread::Info* info) { //FREEZE_ENTITY_POSITION
		Entity entity{ info->Params[0].Int };
		if (!"seamlessTransition"_TC->get(0).toggle || entity != PLAYER::PLAYER_PED_ID()) {
			ENTITY::FREEZE_ENTITY_POSITION(entity, info->Params[1].Int);
		}
	});
	createNativeShim("maintransition"_joaat, 0x8D32347D6D4C40A2, [](rage::scrThread::Info* info) { //NETWORK_RESURRECT_LOCAL_PLAYER
		if (!"seamlessTransition"_TC->get(0).toggle) {
			Vector3 _pos{ info->Params[0].Float, info->Params[1].Float, info->Params[2].Float };
			NETWORK::NETWORK_RESURRECT_LOCAL_PLAYER(_pos, info->Params[3].Float, info->Params[4].Int, info->Params[5].Int, info->Params[6].Int, info->Params[7].Int, info->Params[8].Int);
		}
	});
	createNativeShim("maintransition"_joaat, 0x8D30F648014A92B5, [](rage::scrThread::Info* info) { //GET_EVER_HAD_BAD_PACK_ORDER
		info->ResultPtr->Int = FALSE;
	});
	UnhookWindowsHookEx(*pointers::g_windowHook.add(44).call().as<HHOOK*>());
	m_DX.enable();
	detour::enableAll();
	MH_ApplyQueued();
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