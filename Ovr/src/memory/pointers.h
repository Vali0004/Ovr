#pragma once
#include "pch/pch.h"
#include "rage/classdefs.h"
#include "rage/commands/types.h"

namespace pointers {
	extern void scanAll();
	extern void doPatches();
	namespace types {
		using scrThreadInit = void(*)(rage::scrThread* thread);
		using scrThreadTick = rage::eThreadState(*)(rage::scrThread* thread, u32 opsToExecute);
		using scrThreadKill = void(*)(rage::scrThread* thread);
		using pointerToHandle = Entity(*)(rage::CEntity* ptr);
		using handleToPointer = rage::CEntity*(*)(Entity handle);
		using readBitbufArray = bool(*)(rage::datBitBuffer* buffer, PVOID read, s32 bits, s32 unk);
		using writeBitbufArray = bool(*)(rage::datBitBuffer* buffer, PVOID read, s32 bits, s32 unk);
		using writeBitsSingle = s64(*)(u8* data, u32 val, s32 bits, s32 start);
		using cTaskJumpConstructor = void*(*)(u64 _This, u32 Flags);
		using cTaskFallConstructor = void*(*)(u64 _This, u32 Flags);
		using runAsyncModuleRequest = void(*)(u64* Module);
		using hasIntervalElapsed = bool(*)(u32 Timestamp, u32 Interval);
		using gameSkeletonRunUpdate = u64(*)(rage::gameSkeleton* _This, s32 Type);
		using gameSkeletonUpdateBaseRunGroup = u64(*)(rage::gameSkeletonUpdateBase* _This);
		using dispatchEvent = bool(*)(u64 _This, rage::netConMgr* pConMgr, rage::netConnection::InFrame* pEvent);
		using scGetGameInfoIndex = int(*)(const char* StringId, u64 Unk, u32 GameId);
	}
	inline types::scrThreadInit g_scrThreadInit{};
	inline types::scrThreadTick g_scrThreadTick{};
	inline types::scrThreadKill g_scrThreadKill{};
	inline types::handleToPointer g_handleToPointer{};
	inline types::pointerToHandle g_pointerToHandle{};
	inline types::readBitbufArray g_readBitbufArray{};
	inline types::writeBitbufArray g_writeBitbufArray{};
	inline types::writeBitsSingle g_writeBitsSingle{};
	inline types::cTaskJumpConstructor g_cTaskJumpConstructor{};
	inline types::cTaskFallConstructor g_cTaskFallConstructor{};
	inline types::runAsyncModuleRequest g_runAsyncModuleRequest{};
	inline types::hasIntervalElapsed g_hasIntervalElapsed{};
	inline types::gameSkeletonRunUpdate g_gameSkeletonRunUpdate{};
	inline types::gameSkeletonUpdateBaseRunGroup g_gameSkeletonUpdateBaseRunGroup{};
	inline types::dispatchEvent g_dispatchEvent{};
	inline types::scGetGameInfoIndex g_scGetGameInfoIndex{};

	inline ScGameInfo* g_scGameInfo{};
	inline FriendRegistry* g_friendRegistry{};
	inline ScInfo* g_scInfo{};
	inline CNetworkPlayerMgr** g_networkPlayerMgr{};
	inline Network** g_network{};
	inline CPedFactory** g_pedFactory{};
	inline CReplayInterface** g_replayInterface{};
	inline CGameScriptHandlerMgr* g_scriptHandlerMgr{};
	inline rage::scrProgramTable* g_scrProgramTable{};
	inline rage::scrNativeRegistrationTable* g_nativeRegistrationTable{};
	inline IDXGISwapChain** g_swapChain{};
	inline HashTable<rage::CBaseModelInfo*>* g_hashTable{};
	inline rage::atArray<GtaThread*>* g_gtaThreads{};
	inline rage::scrValue** g_globals{};
	inline eGameState* g_gameState{};
	inline uint32_t* g_threadId{};
	inline uint32_t* g_threadCount{};
	inline u64* g_reportModule{};
	inline void* g_jmpRbxRegister{};
	inline HWND g_hwnd{};
}