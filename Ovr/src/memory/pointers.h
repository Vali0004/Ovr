#pragma once
#include "pch/pch.h"
#include "rage/classes.h"
#include "rage/commands/types.h"
#include "core/logger.h"
#include "memory/scanner.h"
#include "memory/patch.h"

namespace pointers {
	inline int g_scanState{};
	extern void scanSegment(ccp seg);
	extern void doPatches();
	namespace types {
		using scrThreadInit = void(*)(rage::scrThread* thread);
		using scrThreadTick = rage::eThreadState(*)(rage::scrThread* thread, u32 opsToExecute);
		using scrThreadKill = void(*)(rage::scrThread* thread);
		using pointerToHandle = Entity(*)(rage::CEntity* ptr);
		using handleToPointer = rage::CEntity*(*)(Entity handle);
		using readBitbufArray = bool(*)(rage::datBitBuffer* Buffer, PVOID Read, i32 Bits, i32 Unk);
		using writeBitbufArray = bool(*)(rage::datBitBuffer* Buffer, PVOID Read, i32 Bits, i32 Unk);
		using readBitsSingle = bool(*)(u8* Data, u32* Value, i32 Bits, i32 Start);
		using writeBitsSingle = i64(*)(u8* Data, u32 Value, i32 Bits, i32 Start);
		using cTaskJumpConstructor = void*(*)(u64 _This, u32 Flags);
		using cTaskFallConstructor = void*(*)(u64 _This, u32 Flags);
		using runAsyncModuleRequest = void(*)(u64* Module);
		using hasIntervalElapsed = bool(*)(u32 Timestamp, u32 Interval);
		using dispatchEvent = bool(*)(u64 _This, rage::netConnectionManager* pConMgr, rage::netConnection::InFrame* pEvent);
		using scriptVm = rage::eThreadState(*)(rage::scrValue* stack, rage::scrValue** globals, rage::scrProgram* pt, rage::scrThread::Serialised* ser);
		using scGetGameInfoIndex = int(*)(const char* StringId, u64 Unk, u32 GameId);
		using joinBySessionInfo = bool(*)(CNetwork* Network, rage::rlSessionInfo* Info, i32 Unk, i32 Flags, rage::rlGamerHandle* Handles, i32 HandleCount);
		using proccessPackedEvents = void(*)(rage::netEventMgr* pEventMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, u16 Id, i32 Index, i32 HandledBitset, i32 BufferSize, rage::datBitBuffer* Buffer);
		using sendEventAck = void(*)(rage::netEventMgr* pEventMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, i32 Index, i32 HandledBitset);
		using getNetObject = rage::netObject* (*)(CNetworkObjectMgr* pObjMgr, u16 ObjectId, bool CanDeleteBePending);
		using receiveCloneSync = eAckCode(*)(CNetworkObjectMgr* pObjMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, eNetObjectType ObjectType, u16 ObjectId, rage::datBitBuffer* Buffer, u16 Unknown, u32 Timestamp);
		using receiveCloneCreate = bool(*)(CNetworkObjectMgr* pObjMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, eNetObjectType ObjectType, i32 ObjectId, i32 ObjectFlag, rage::datBitBuffer* Buffer, i32 Timestamp);
		using canApplyData = bool(*)(rage::netSyncTree* pSyncTree, rage::netObject* pObject);
		using getSyncTreeForType = rage::netSyncTree*(*)(CNetworkObjectMgr* pObjMgr, u16 SyncType);
		using getGamerTaskResult = bool(*)(i32 ProfileIndex, rage::rlGamerHandle* pHandles, i32 Count, rage::rlSessionByGamerTaskResult* pResult, i32 Unk, bool* pSuccess, rage::rlTaskStatus* pStatus);
		using findGameMatch = bool(*)(i32 ProfileIndex, i32 AvailableSlots, NetworkGameFilterMatchmakingComponent* pFilter, u32 Count, rage::rlSessionInfo* pSessions, i32* OutputSize, rage::rlTaskStatus* pStatus);
		using addItemToBasket = bool(*)(CNetworkShoppingMgr* pTransactionMgr, i32* Items);
		using constructBasket = bool(*)(CNetworkShoppingMgr* pTransactionMgr, i32* pId, u32 Category, u32 Action, u32 Target);
		using beginService = bool(*)(CNetworkShoppingMgr* pTransactionMgr, i32* pId, u32 Service, u32 Category, u32 Item, u32 Action, i32 Value, u32 Target);
		using request = bool(*)(CHttpRequest* pRequest);
		using sendMetric = bool(*)(rage::rlMetric* pMetric, bool Unk);
		using sendNetworkEvent = void(*)(rage::netEventMgr* pEventMgr, rage::netGameEvent* pEvent);
		using processMatchmakingFind = bool(*)(u64* _This, u64* Unused, rage::JSONNode* pNode, i32* Unk);
		using triggerPlayermenuAction = bool(*)(CPlayerListMenu* pMenu, u32* pHash);
		using getFriendsMenu = u64(*)(u32 Index);
		using getArchetype = rage::fwArchetype*(*)(u32 Hash, u64* pUnk);
		using calculateMipLevel = i32(*)(u8 Type, u16 Width, u16 Height, u8 Levels, u32 Format);
		using getStreamingModuleFromExtension = rage::strStreamingModule* (*)(rage::strStreamingModuleMgr* pMgr, cc* Extension);
		using insertStreamingModule = i32(*)(rage::strStreamingModuleMgr* pMgr, rage::strStreamingModule* pModule);
		using hasRosPrivilege = bool(*)(u64* _This, i32 Privilege);
		using updateTimecycleData = i64(*)(u64* _This, TimecycleKeyframeData* pData);
		using allocateReliable = void* (*)(rage::netConnection* pCon, i32 RequiredMemory);
		using conMgrTryFree = void(*)(rage::netConnectionManager* pConMgr);
		using removeMessageFromQueue = void(*)(rage::netMessageQueue* pQueue, rage::netQueuedMessage* pMsg);
		using removeMessageFromUnacknowledgedReliables = void(*)(rage::netQueuedMessage** pList, u16* pUnk);
	}
	inline types::scrThreadInit g_scrThreadInit{};
	inline types::scrThreadTick g_scrThreadTick{};
	inline types::scrThreadKill g_scrThreadKill{};
	inline types::handleToPointer g_handleToPointer{};
	inline types::pointerToHandle g_pointerToHandle{};
	inline types::readBitbufArray g_readBitbufArray{};
	inline types::writeBitbufArray g_writeBitbufArray{};
	inline types::readBitsSingle g_readBitsSingle{};
	inline types::writeBitsSingle g_writeBitsSingle{};
	inline types::cTaskJumpConstructor g_cTaskJumpConstructor{};
	inline types::cTaskFallConstructor g_cTaskFallConstructor{};
	inline types::runAsyncModuleRequest g_runAsyncModuleRequest{};
	inline types::hasIntervalElapsed g_hasIntervalElapsed{};
	inline types::dispatchEvent g_dispatchEvent{};
	inline types::scriptVm g_scriptVm{};
	inline types::scGetGameInfoIndex g_scGetGameInfoIndex{};
	inline types::joinBySessionInfo g_joinBySessionInfo{};
	inline types::proccessPackedEvents g_proccessPackedEvents{};
	inline types::sendEventAck g_sendEventAck{};
	inline types::getNetObject g_getNetObject{};
	inline types::receiveCloneSync g_receiveCloneSync{};
	inline types::receiveCloneCreate g_receiveCloneCreate{};
	inline types::canApplyData g_canApplyData{};
	inline types::getSyncTreeForType g_getSyncTreeForType{};
	inline types::getGamerTaskResult g_getGamerTaskResult{};
	inline types::findGameMatch g_findGameMatch{};
	inline types::addItemToBasket g_addItemToBasket{};
	inline types::beginService g_beginService{};
	inline types::constructBasket g_constructBasket{};
	inline types::request g_request{};
	inline types::sendMetric g_sendMetric{};
	inline types::sendNetworkEvent g_sendNetworkEvent{};
	inline types::processMatchmakingFind g_processMatchmakingFind{};
	inline types::triggerPlayermenuAction g_triggerPlayermenuAction{};
	inline types::getFriendsMenu g_getFriendsMenu{};
	inline types::getArchetype g_getArchetype{};
	inline types::calculateMipLevel g_calculateMipLevel{};
	inline types::getStreamingModuleFromExtension g_getStreamingModuleFromExtension{};
	inline types::insertStreamingModule g_insertStreamingModule{};
	inline types::hasRosPrivilege g_hasRosPrivilege{};
	inline types::updateTimecycleData g_updateTimecycleData{};
	inline types::allocateReliable g_allocateReliable{};
	inline types::conMgrTryFree g_conMgrTryFree{};
	inline types::removeMessageFromQueue g_removeMessageFromQueue{};
	inline types::removeMessageFromUnacknowledgedReliables g_removeMessageFromUnacknowledgedReliables{};

	inline rage::grcTextureStore* g_textureStore{};
	inline CStreaming* g_streaming{};
	inline ScGameInfo* g_scGameInfo{};
	inline FriendRegistry* g_friendRegistry{};
	inline ScInfo* g_scInfo{};
	inline CNetworkShoppingMgr** g_networkShoppingMgr{};
	inline CNetworkPlayerMgr** g_networkPlayerMgr{};
	inline CNetworkObjectMgr** g_networkObjectMgr{};
	inline CNetwork** g_network{};
	inline CPedFactory** g_pedFactory{};
	inline CReplayInterface** g_replayInterface{};
	inline CGameScriptHandlerMgr* g_scriptHandlerMgr{};
	inline rage::scrProgramTable* g_scrProgramTable{};
	inline rage::scrNativeRegistrationTable* g_nativeRegistrationTable{};
	inline IDXGISwapChain** g_swapChain{};
	inline PresenceData** g_presenceData{};
	inline HashTable<rage::CBaseModelInfo*>* g_hashTable{};
	inline rage::atArray<GtaThread*>* g_gtaThreads{};
	inline rage::scrValue** g_globals{};
	inline eLoadingScreenState* g_loadingScreenState{};
	inline uint32_t* g_threadId{};
	inline uint32_t* g_threadCount{};
	inline u64* g_reportModule{};
	inline u64 g_nativeRegistration{};
	inline u64 g_vehicleModelInfoVtbl{};
	inline mem g_windowHook{};
	inline HWND g_hwnd{};
}