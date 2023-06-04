#include "memory/pointers.h"
#include "memory/scanner.h"
#include "memory/patch.h"

namespace pointers {
	void scanAll() {
        g_scrThreadInit = scan("STI", "83 89 ? ? ? ? ? 83 A1 ? ? ? ? ? 80 A1 ? ? ? ? ?").as<decltype(g_scrThreadInit)>();
        g_scrThreadTick = scan("STT", "80 B9 ? ? ? ? ? 8B FA 48 8B D9 74 05").sub(0xF).as<decltype(g_scrThreadTick)>();
        g_scrThreadKill = scan("STK", "48 89 5C 24 ? 57 48 83 EC 20 48 83 B9 ? ? ? ? ? 48 8B D9 74 14").as<decltype(g_scrThreadKill)>();
        g_handleToPointer = scan("HTP", "83 F9 FF 74 31 4C 8B 0D").as<decltype(g_handleToPointer)>();
        g_pointerToHandle = scan("PTH", "48 8D 1C F8 48 8B CB").add(7).call().as<decltype(g_pointerToHandle)>();
        g_readBitbufArray = scan("RBA", "48 89 5C 24 ? 57 48 83 EC 30 41 8B F8 4C").as<decltype(g_readBitbufArray)>();
        g_writeBitbufArray = scan("WBA", "E8 ? ? ? ? 01 7E 08").call().as<decltype(g_writeBitbufArray)>();
        g_writeBitsSingle = scan("WBS", "E8 ? ? ? ? 01 6B 10").call().as<decltype(g_writeBitsSingle)>();
        g_cTaskJumpConstructor = scan("CTJC", "48 89 5C 24 ? 89 54 24 10 57 48 83 EC 30 0F 29 74 24").as<decltype(g_cTaskJumpConstructor)>();
        g_cTaskFallConstructor = scan("CTFC", "E8 ? ? ? ? B3 04 08 98 A0").call().as<decltype(g_cTaskFallConstructor)>();
        g_runAsyncModuleRequest = scan("RAMS", "48 89 5C 24 ? 57 48 83 EC ? 8B FA 8B D9 FF 15").as<decltype(g_runAsyncModuleRequest)>();
        g_hasIntervalElapsed = scan("HIE", "48 89 5C 24 ? 57 48 83 EC ? 0F B6 99").as<decltype(g_hasIntervalElapsed)>();
        g_gameSkeletonRunUpdate = scan("GSRU", "48 8D 0D ? ? ? ? BA 02 00 00 00 84 DB 75 05").sub(17).call().as<decltype(g_gameSkeletonRunUpdate)>();
        g_gameSkeletonUpdateBaseRunGroup = scan("GSUBRG", "40 53 48 83 EC 20 48 8B 59 20 EB 0D 48 8B 03 48").as<decltype(g_gameSkeletonUpdateBaseRunGroup)>();
        g_dispatchEvent = scan("DE", "48 83 EC 20 4C 8B 71 50 33 ED").sub(0x19).as<decltype(g_dispatchEvent)>();
        g_scGetGameInfoIndex = scan("SGGII", "E8 ? ? ? ? 85 C0 78 27 3B 47 14 7D 1E 48 98 48 8B D6 48 69 C8", { "socialclub.dll" }).call().as<decltype(g_scGetGameInfoIndex)>();

        g_scGameInfo = scan("SGI", "48 8D 05 ? ? ? ? 48 03 F8 44 8B 47 14 48 8D 57 20 E8 ? ? ? ? 85", { "socialclub.dll" }).mov().as<decltype(g_scGameInfo)>();
        g_friendRegistry = scan("FR", "41 8B F4 3B C5").sub(0xB).lea().as<decltype(g_friendRegistry)>();
        g_scInfo = scan("SI", "48 8D 05 ? ? ? ? 48 63 D1").mov().as<decltype(g_scInfo)>();
        g_networkPlayerMgr = scan("NPM", "48 8B 0D ? ? ? ? 8A D3 48 8B 01 FF 50 ? 4C 8B 07 48 8B CF").mov().as<decltype(g_networkPlayerMgr)>();
        g_network = scan("N", "48 8B 0D ? ? ? ? 48 8D 05 ? ? ? ? 45 33 C9").mov().as<decltype(g_network)>();
        g_pedFactory = scan("PF", "48 8B 05 ? ? ? ? 8A D1").mov().as<decltype(g_pedFactory)>();
        g_replayInterface = scan("RI", "0F B7 44 24 ? 66 89 44 4E").add(0x1C).mov().as<decltype(g_replayInterface)>();
        g_scriptHandlerMgr = scan("SHM", "74 17 48 8B C8 E8 ? ? ? ? 48 8D 0D").add(0xA).mov().as<decltype(g_scriptHandlerMgr)>();
        g_scrProgramTable = scan("SPT", "48 8B 1D ? ? ? ? 41 83 F8 FF").mov().as<decltype(g_scrProgramTable)>();
        g_nativeRegistrationTable = scan("NRT", "40 B6 01 39 79 2C").add(0xC).mov().as<decltype(g_nativeRegistrationTable)>();
        g_swapChain = scan("SC", "48 8B 3D ? ? ? ? 3B C8").mov().as<decltype(g_swapChain)>();
        g_hashTable = scan("MT", "4C 03 05 ? ? ? ? EB 03").mov().as<decltype(g_hashTable)>();
        g_gtaThreads = scan("GT", "F5 8B FD 73").add(5).mov().as<decltype(g_gtaThreads)>();
        g_globals = scan("G", "48 8B 8D ? ? ? ? 4C 8D 4D 08").add(0xB).mov().as<decltype(g_globals)>();
        g_gameState = scan("GS", "83 3D ? ? ? ? ? 75 17 8B 43 20 25").lea().add(1).as<decltype(g_gameState)>();
        g_threadId = scan("TI", "8B 15 ? ? ? ? 48 8B 05 ? ? ? ? FF C2 89 15 ? ? ? ? 48 8B 0C D8").lea().as<decltype(g_threadId)>();
        g_threadCount = scan("TC", "FF 0D ? ? ? ? 48 8B D9 75").lea().as<decltype(g_threadCount)>();
        g_reportModule = scan("RM", "48 8D 0D ? ? ? ? 88 05 ? ? ? ? 48 8D 05").mov().as<decltype(g_reportModule)>();
        g_jmpRbxRegister = scan("JRR", "23 C3 C1 E0 0C").sub(1).as<decltype(g_jmpRbxRegister)>();
        g_hwnd = FindWindowA("grcWindow", nullptr);
    }
    void doPatches() {
        //g_patches.add("RTUBS", scan("RTUBS", "42 6F 6E 75 73 00").as<u64>(), 
        //    { 0x42, 0x6F, 0x6E, 0x75, 0x73, 0x61 }
        //);
        //g_patches.add("RTULS", scan("RTULS", "62 6F 6E 75 73 00").as<u64>(), 
        //    { 0x62, 0x6F, 0x6E, 0x75, 0x73, 0x61 }
        //);
        //g_patches.add("S3I", scan("S3I", "BA ? ? ? ? E8 ? ? ? ? 84 C0 74 ? BB ? ? ? ? 8A C3 48 83 C4 ? 5B C3 90").as<u64>(), 
        //    { 0xBA, 0xB9, 0x0B, 0x00, 0x00 }
        //);
        //g_patches.add("S30I", scan("S30I", "BA ? ? ? ? E8 ? ? ? ? 84 C0 74 ? BB ? ? ? ? 8A C3 48 83 C4 ? 5B C3 CC 48 83 EC").as<u64>(), 
        //    { 0xBA, 0x31, 0x75, 0x00, 0x00 }
        //);
        //g_patches.add("ISMV", scan("ISMV", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 54 41 55 41 56 41 57 48 83 EC 20 45 0F").as<u64>(), 
        //    { 0xB0, 0x01, 0xC3 }
        //);
    }
}