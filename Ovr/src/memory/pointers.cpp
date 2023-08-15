#include "memory/pointers.h"
#include "util/budgeting_fix.h"

namespace pointers {
	bool scanAll() {
		g_loadingScreenState = scan("LSS", "83 3D ? ? ? ? ? 75 17 8B 43 20 25").lea().add(1).as<decltype(g_loadingScreenState)>();
		switch (*pointers::g_loadingScreenState) {
		case eLoadingScreenState::PreLegal: {
			std::this_thread::sleep_for(24s);
			*pointers::g_loadingScreenState = eLoadingScreenState::Legals;
			std::this_thread::sleep_for(500ms);
		} break;
		case eLoadingScreenState::Legals: {
			std::this_thread::sleep_for(1s);
			*pointers::g_loadingScreenState = eLoadingScreenState::LandingPage;
		} break;
		}
		g_scrThreadInit = scan("STI", "83 89 ? ? ? ? ? 83 A1 ? ? ? ? ? 80 A1 ? ? ? ? ?").as<decltype(g_scrThreadInit)>();
		g_scrThreadTick = scan("STT", "80 B9 ? ? ? ? ? 8B FA 48 8B D9 74 05").sub(0xF).as<decltype(g_scrThreadTick)>();
		g_scrThreadKill = scan("STK", "48 89 5C 24 ? 57 48 83 EC 20 48 83 B9 ? ? ? ? ? 48 8B D9 74 14").as<decltype(g_scrThreadKill)>();
		g_handleToPointer = scan("HTP", "83 F9 FF 74 31 4C 8B 0D").as<decltype(g_handleToPointer)>();
		g_readBitbufArray = scan("RBA", "48 89 5C 24 ? 57 48 83 EC 30 41 8B F8 4C").as<decltype(g_readBitbufArray)>();
		g_writeBitbufArray = scan("WBA", "E8 ? ? ? ? 01 7E 08").call().as<decltype(g_writeBitbufArray)>();
		g_readBitsSingle = scan("RBS", "48 8D 54 24 ? 41 C1 E1 03").add(0xF).call().as<decltype(g_readBitsSingle)>();
		g_writeBitsSingle = scan("WBS", "E8 ? ? ? ? 01 6B 10").call().as<decltype(g_writeBitsSingle)>();
		g_cTaskJumpConstructor = scan("CTJC", "48 89 5C 24 ? 89 54 24 10 57 48 83 EC 30 0F 29 74 24").as<decltype(g_cTaskJumpConstructor)>();
		g_cTaskFallConstructor = scan("CTFC", "E8 ? ? ? ? B3 04 08 98 A0").call().as<decltype(g_cTaskFallConstructor)>();
		g_runAsyncModuleRequest = scan("RAMR", "48 89 5C 24 ? 57 48 83 EC ? 0F B6 99").as<decltype(g_runAsyncModuleRequest)>();
		g_hasIntervalElapsed = scan("HIE", "48 89 5C 24 ? 57 48 83 EC ? 8B FA 8B D9 FF 15").as<decltype(g_hasIntervalElapsed)>();
		g_dispatchEvent = scan("DE", "48 83 EC 20 4C 8B 71 50 33 ED").sub(0x19).as<decltype(g_dispatchEvent)>();
		g_scriptVm = scan("SV", "4A 89 3C 39").sub(0x27).call().as<decltype(g_scriptVm)>();
		g_scGetGameInfoIndex = scan("SGGII", "E8 ? ? ? ? 85 C0 78 27 3B 47 14 7D 1E 48 98 48 8B D6 48 69 C8", { "socialclub.dll" }).call().as<decltype(g_scGetGameInfoIndex)>();
		g_joinBySessionInfo = scan("JBSI", "E8 ? ? ? ? 0F B6 CB 84 C0 41 0F 44 CD").call().as<decltype(g_joinBySessionInfo)>();
		g_proccessPackedEvents = scan("PPE", "66 41 83 F9 ? 0F 83").as<decltype(g_proccessPackedEvents)>();
		g_sendEventAck = scan("SEA", "48 8B DA 44 8B CD").add(0x13).call().as<decltype(g_sendEventAck)>();
		g_getNetObject = scan("GNO", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 54 41 55 41 56 41 57 48 83 EC 40 4C 8B EA").add(0x109).call().as<decltype(g_getNetObject)>();
		g_receiveCloneSync = scan("RCS", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 54 41 55 41 56 41 57 48 83 EC 40 4C 8B EA").as<decltype(g_receiveCloneSync)>();
		g_receiveCloneCreate = scan("RCC", "48 8B C4 66 44 89 48").as<decltype(g_receiveCloneCreate)>();
		g_canApplyData = scan("CAD", "E8 ? ? ? ? 84 C0 0F 84 AF 01 00 00 48 8B 03").call().as<decltype(g_canApplyData)>();
		g_getSyncTreeForType = scan("GSTFT", "0F B7 CA 83 F9 07").as<decltype(g_getSyncTreeForType)>();
		g_getGamerTaskResult = scan("GGTR", "E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 8B 05 ? ? ? ? 48 8D 4C 24").call().as<decltype(g_getGamerTaskResult)>();
		g_findGameMatch = scan("FGM", "E8 ? ? ? ? 84 C0 0F 84 F6 FE FF FF").call().as<decltype(g_findGameMatch)>();
		g_addItemToBasket = scan("AITB", "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B F2 48 8D 54 24").as<decltype(g_addItemToBasket)>();
		g_beginService = scan("BS", "54 E9 9B 9B 94").as<decltype(g_beginService)>();
		g_constructBasket = scan("CB", "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 54 41 56 41 57 48 83 EC 30 48 8B F1").as<decltype(g_constructBasket)>();
		g_request = scan("R", "40 55 57 41 54 41 55").as<decltype(g_request)>();
		g_sendMetric = scan("SM", "E8 ? ? ? ? EB 69 41 8B D6").call().as<decltype(g_sendMetric)>();
		g_sendNetworkEvent = scan("SNE", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 54 41 55 41 56 41 57 48 83 EC 30 48 8D 71 28").as<decltype(g_sendNetworkEvent)>();
		g_processMatchmakingFind = scan("PMF", "48 89 5C 24 08 48 89 74 24 10 57 48 81 EC F0 00 00 00 41 83").as<decltype(g_processMatchmakingFind)>();
		g_triggerPlayermenuAction = scan("TPA", "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B D9").as<decltype(g_triggerPlayermenuAction)>();
		g_getFriendsMenu = scan("GFM", "75 1C E8 ? ? ? ? 48 85 C0").sub(0xB).as<decltype(g_getFriendsMenu)>();
		g_getArchetype = scan("GA", "89 44 24 40 8B 4F 08 80 E3 01 E8").add(0xA).call().as<decltype(g_getArchetype)>();
		g_calculateMipLevel = scan("CML", "8B 47 58 0F B6 49 5C 89 44 24 20 E8").add(0xB).call().as<decltype(g_calculateMipLevel)>();
		g_getStreamingModuleFromExtension = scan("GSMFE", "40 53 48 83 EC 20 48 8B C2 48 8B D9 33 D2 48 8B C8 E8 ? ? ? ? 33 D2 44 8B D0 F7 35 ? ? ? ? 44 8B C2 48 8B 15 ? ? ? ? 46 8B 0C 82 41").as<decltype(g_getStreamingModuleFromExtension)>();
		g_insertStreamingModule = scan("ISM", "76 16 48 8B 41 18 44 0F B7 41 20 4E").sub(0xF).as<decltype(g_insertStreamingModule)>();
		g_hasRosPrivilege = scan("HRP", "E8 ? ? ? ? EB 0B 8B CB").call().as<decltype(g_hasRosPrivilege)>();
		g_updateTimecycleData = scan("TCD", "48 83 EC 18 48 8B 0D").as<decltype(g_updateTimecycleData)>();
		g_allocateReliable = scan("AR", "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 20 48 8B D9 48 8B 49 18").as<decltype(g_allocateReliable)>();
		g_conMgrTryFree = scan("CMTR", "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 20 48 8B D9 48 8B 49 18").add(0x51).call().as<decltype(g_conMgrTryFree)>();
		g_removeMessageFromQueue = scan("RMFQ", "E8 ? ? ? ? 0F B7 43 4C 48 8D 55 20").call().as<decltype(g_removeMessageFromQueue)>();
		g_removeMessageFromUnacknowledgedReliables = scan("RMFUR", "E8 ? ? ? ? 0F B7 43 4C 48 8D 55 20").add(0x18).call().as<decltype(g_removeMessageFromUnacknowledgedReliables)>();
		g_sendPresenceEvent = scan("SPE", "E8 ? ? ? ? EB 02 B0 01 48 8B 9C 24").call().as<decltype(g_sendPresenceEvent)>();
		g_writePlayerGameStateDataNode = scan("WPGSDN", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC 30 0F B7 81").as<decltype(g_writePlayerGameStateDataNode)>();
		g_queuePacketViaMsgId = scan("QPVMI", "E8 ? ? ? ? 84 C0 74 4D B3 01").call().as<decltype(g_queuePacketViaMsgId)>();
		g_getServerData = scan("GSD", "BA ? ? ? ? 48 8B D9 E8 ? ? ? ? 48 8B D0").sub(0x16).as<decltype(g_getServerData)>();
		g_getNewsStory = scan("GNS", "48 89 5C 24 18 48 89 74 24 20 55 57 41 56 48 8B EC 48 81 EC 80 00 00 00 48 8D").as<decltype(g_getNewsStory)>();
		g_updateVideoMemoryBar = scan("UVMB", "BE 06 00 00 00 8B CE E8").sub(0x6D).as<decltype(g_updateVideoMemoryBar)>();
		g_getAvailableMemoryForStreamer = scan("GAMFS", "E8 ? ? ? ? 48 8D 0C 3B 48 3B C1").call().as<decltype(g_getAvailableMemoryForStreamer)>();
		g_settingsVramTex = scan("SVT", "B9 84 04 00 00 41 B9 6B").sub(0x6D).as<decltype(g_settingsVramTex)>();
		g_hasGameBeenAltered = scan("HGBA", "40 53 48 83 EC 30 48 8B 1D ? ? ? ? BA").as<decltype(g_hasGameBeenAltered)>();
		g_resourceError = scan("RE", "E8 ? ? ? ? CC FF 15 ? ? ? ?").call().as<decltype(g_resourceError)>();
		g_callResourceError = scan("CRE", "48 83 EC 28 33 D2 E8 ? ? ? ? CC").as<decltype(g_callResourceError)>();

		g_textureStore = scan("TS", "48 8D 0D ? ? ? ? E8 ? ? ? ? 8B 45 EC 4C 8D 45 F0 48 8D 55 EC 48 8D 0D ? ? ? ? 89 45 F0 E8").mov().as<decltype(g_textureStore)>();
		g_streaming = scan("S", "48 8D 0D ? ? ? ? 03 D3 E8 ? ? ? ? 66 44 39 7D ? 74 09 48 8B 4D E8 E8").mov().as<decltype(g_streaming)>();
		g_scGameInfo = scan("SGI", "48 8D 05 ? ? ? ? 48 03 F8 44 8B 47 14 48 8D 57 20 E8 ? ? ? ? 85", { "socialclub.dll" }).mov().as<decltype(g_scGameInfo)>();
		g_presenceData = scan("PD", "48 8D 05 ? ? ? ? 48 8B F1 48 89 01 48 83 C1 08 E8 ? ? ? ? 33 ED 48 8D 8E 68 5B 00 00", { "socialclub.dll" }).mov().as<decltype(g_presenceData)>();
		g_friendRegistry = scan("FR", "41 8B F4 3B C5").sub(0xB).lea().as<decltype(g_friendRegistry)>();
		g_scInfo = scan("SI", "48 8B D3 48 8D 4C 24 ? 48 69 D2").sub(7).mov().as<decltype(g_scInfo)>();
		g_networkShoppingMgr = scan("NSM", "48 8B 0D ? ? ? ? 41 8A D9").mov().as<decltype(g_networkShoppingMgr)>();
		g_networkTransactionMgr = scan("NTM", "8B 75 40 48 89 1D ? ? ? ?").add(3).mov().as<decltype(g_networkTransactionMgr)>();
		g_networkPlayerMgr = scan("NPM", "48 8B 0D ? ? ? ? 8A D3 48 8B 01 FF 50 ? 4C 8B 07 48 8B CF").mov().as<decltype(g_networkPlayerMgr)>();
		g_networkObjectMgr = scan("NOM", "48 8B 0D ? ? ? ? 45 33 C0 E8 ? ? ? ? 33 FF 4C 8B F0").mov().as<decltype(g_networkObjectMgr)>();
		g_network = scan("N", "48 8B 0D ? ? ? ? 48 8B D7 E8 ? ? ? ? 84 C0 75 17 48 8B 0D ? ? ? ? 48 8B D7").mov().as<decltype(g_network)>();
		g_pedFactory = scan("PF", "48 8B 05 ? ? ? ? 8A D1").mov().as<decltype(g_pedFactory)>();
		g_replayInterface = scan("RI", "0F B7 44 24 ? 66 89 44 4E").add(0x1C).mov().as<decltype(g_replayInterface)>();
		g_netCatalog = scan("NC", "48 8B 05 ? ? ? ? 48 8B 0C D0 EB 08").mov().as<decltype(g_netCatalog)>();
		g_scriptHandlerMgr = scan("SHM", "74 17 48 8B C8 E8 ? ? ? ? 48 8D 0D").add(0xA).mov().as<decltype(g_scriptHandlerMgr)>();
		g_scrProgramTable = scan("SPT", "48 8B 1D ? ? ? ? 41 83 F8 FF").mov().as<decltype(g_scrProgramTable)>();
		g_nativeRegistrationTable = scan("NRT", "40 B6 01 39 79 2C").add(0xC).mov().as<decltype(g_nativeRegistrationTable)>();
		g_swapChain = scan("SC", "48 8B 3D ? ? ? ? 3B C8").mov().as<decltype(g_swapChain)>();
		g_hashTable = scan("MT", "4C 03 05 ? ? ? ? EB 03").mov().as<decltype(g_hashTable)>();
		g_gtaThreads = scan("GT", "F5 8B FD 73").add(5).mov().as<decltype(g_gtaThreads)>();
		g_globals = scan("G", "48 8B 8D ? ? ? ? 4C 8D 4D 08").add(0xB).mov().as<decltype(g_globals)>();
		g_vfxWheel = scan("VW", "48 8D 05 ? ? ? ? 48 6B FF 45 F3 0F 59 0D ? ? ? ? F3 41 0F 59 9E ? ? ? ? F3 0F 10 BD ? ? ? ? 48 03 FE 48 69 FF ? ? ? ? F3").mov().as<decltype(g_vfxWheel)>();
		g_vfxWheelClassSize = scan("VWCS", "48 8D 05 ? ? ? ? 48 6B FF 45 F3 0F 59 0D ? ? ? ? F3 41 0F 59 9E ? ? ? ? F3 0F 10 BD ? ? ? ? 48 03 FE 48 69 FF ? ? ? ? F3").add(0xA).as<decltype(g_vfxWheelClassSize)>();
		g_threadId = scan("TI", "8B 15 ? ? ? ? 48 8B 05 ? ? ? ? FF C2 89 15 ? ? ? ? 48 8B 0C D8").lea().as<decltype(g_threadId)>();
		g_threadCount = scan("TC", "FF 0D ? ? ? ? 48 8B D9 75").lea().as<decltype(g_threadCount)>();
		g_nativeRegistration = scan("NR", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 BA 10 00 00 00 B9 20 03 00 00").add(0x1E).call().as<decltype(g_nativeRegistration)>();
		g_vehicleModelInfoVtbl = scan("VMIV", "45 33 C0 48 8D 05 ? ? ? ? 48 8D BB C0 00 00 00").add(3).mov().as<decltype(g_vehicleModelInfoVtbl)>();
		g_pedModelInfoVtbl = scan("PMIV", "E3 FF FF 48 8D 05 ? ? ? ? 48 89 03 33 C0 48 89 83").add(3).mov().as<decltype(g_pedModelInfoVtbl)>();
		g_forceHost = scan("FH", "C6 05 ? ? ? ? ? 48 8B CB E8 ? ? ? ? 84 C0 75 08").mov().as<decltype(g_forceHost)>();
		g_registerDecorNotAllowed = scan("RDNA", "0F B6 05 ? ? ? ? 33 D2 83 F9 08 0F 44 C2 88 05 ? ? ? ? C3").add(0x10).call().as<decltype(g_registerDecorNotAllowed)>();
		g_vramLocation = scan("VL", "4C 63 C0 48 8D 05 ? ? ? ? 48 8D 14").add(3).mov().as<decltype(g_vramLocation)>();
		g_allocatorAmount = scan("AA", "41 B8 00 00 00 40 48 8B D5 89").add(2).as<decltype(g_allocatorAmount)>();
		g_hwnd = FindWindowA("grcWindow", nullptr);
		return true;
	}
	void doPatches() {
		try {
			for (auto& h : getAllResults("48 8D 45 ? 48 89 45 ? 48 8D 05 ? ? ? ? 48 89 45")) {
				mem target{ h.add(8) };
				g_arxPatches.addInteg(target.as<u8*>());
			}
			if (g_arxPatches.count()) {
				LOG_DEBUG("Patched {} ARX functions ({} checkers)", g_arxPatches.count(), g_arxPatches.integCount());
			}
			else {
				LOG_DEBUG("ARX functions were already patched.");
			}
		}
		catch (...) {
			LOG_DEBUG("ARX function patches failed to patch, checking if they were already applied.");
		}
		//Fix EXE path for Code EXE size
		std::vector<u8> moduleName(MAX_PATH + 1);
		GetModuleFileNameA(GetModuleHandleA(0), (char*)moduleName.data(), static_cast<DWORD>(moduleName.size()));
		if (mem target{ scan("CCRCEP", "41 BD 01 00 00 00 48 89 05").add(9).rip() }; *target.as<u8**>() != moduleName.data()) {
			g_patches.add("CCRCEP", *target.as<u8**>(), moduleName, true);
		}
		//Is matchmaking session valid
		g_patches.add("ISMSV", scan("ISMSV", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 54 41 55 41 56 41 57 48 83 EC 20 45 0F").as<u8*>(), { 0xB0ui8, 0x01ui8, 0xC3ui8 });
		//Free Event Error
		g_patches.add("FEE", scan("FEE", "48 8B 5C 24 40 48 8B 6C 24 48 48 8B 74 24 50 48 8B 7C 24 58 48 83 C4 30 41 5E C3 48 8B 0D").add(0x31).as<u8*>(), { 0x90ui8, 0x90ui8, 0x90ui8, 0x90ui8, 0x90ui8 });
		//Crash trigger
		g_patches.add("MTC", scan("MTC", "48 3B F8 74 ? 8B 1D").add(4).as<u8*>(), { 0x00ui8 });
		//Windows hook to block use of WIN, we are storing the pointer and patching the handler to fix this.
		g_windowHook = scan("WH", "48 83 EC 28 33 C9 FF 15 ? ? ? ? 45 33 C9");
		g_patches.add("WH", g_windowHook.as<u8*>(), { 0xC3ui8, 0x90ui8, 0x90ui8, 0x90ui8 });
		//Fix extended budgeting
		budgeting::init();
	}
}