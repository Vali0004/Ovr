#pragma once
#include "pch/pch.h"
#include "core/logger.h"
#include "memory/pointers.h"
#include "rage/classes.h"
#include "rage/commands/list.h"
#include "methods/vmt.h"
#include "methods/detour.h"
#include "shv/dynamic_loader.h"
#include "rage/joaat.h"
#include "util/player_mgr.h"
#include "util/statistics.h"
#define CALL(hk, ...) g_hooking->m_##hk##.getOg<pointers::types::##hk##>()(__VA_ARGS__)
#define CALL_DECL(hk, ...) g_hooking->m_##hk##.getOg<decltype(&##hk)>()(__VA_ARGS__)
#define RET_CALL(hk, ...) return CALL(hk, __VA_ARGS__);
#define RET_CALL_DECL(hk, ...) return CALL_DECL(hk, __VA_ARGS__);
#define VMT_CALL(vmt, hook, ...) g_hooking->m_##vmt##.getOg<decltype(&hooks::##hook)>(g_##hook##Index)(__VA_ARGS__)

inline u64 g_swapchainSize{ 19 };
inline u64 g_resizeBuffersIndex{ 13 };
inline u64 g_presentIndex{ 8 };
inline u64 g_updateAttributeIntIndex{ 1 };
inline TimecycleKeyframeData* g_timecycleKeyframeData{};
inline bool g_timecycleKeyframeDataUpdate{ true};
extern std::string getCurrentStreamingName();
extern u32 getCurrentStreamingIndex();
struct hooks {
	static void* cTaskJumpConstructor(u64 _This, u32 Flags);
	static void* cTaskFallConstructor(u64 _This, u32 Flags);
	static void runAsyncModuleRequest(u64* Module);
	static bool hasIntervalElapsed(u32 Timestamp, u32 Interval);
	static bool dispatchEvent(u64 _This, rage::netConMgr* pConMgr, rage::netConnection::InFrame* pEvent);
	static rage::eThreadState scriptVm(rage::scrValue* stack, rage::scrValue** globals, rage::scrProgram* pt, rage::scrThread::Serialised* ser);
	static void proccessPackedEvents(rage::netEventMgr* pEventMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, u16 Id, i32 Index, i32 HandledBitset, i32 BufferSize, rage::datBitBuffer* Buffer);
	static eAckCode receiveCloneSync(CNetworkObjectMgr* pObjMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, eNetObjectType ObjectType, u16 ObjectId, rage::datBitBuffer* Buffer, u16 Unknown, u32 Timestamp);
	static bool receiveCloneCreate(CNetworkObjectMgr* pObjMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, eNetObjectType ObjectType, i32 ObjectId, i32 ObjectFlag, rage::datBitBuffer* Buffer, i32 Timestamp);
	static bool canApplyData(rage::netSyncTree* pSyncTree, rage::netObject* pObject);
	static bool findGameMatch(i32 ProfileIndex, i32 AvailableSlots, NetworkGameFilterMatchmakingComponent* pFilter, u32 Count, rage::rlSessionInfo* pSessions, i32* OutputSize, rage::rlTaskStatus* pStatus);
	static bool processMatchmakingFind(u64* _This, u64* Unused, rage::JSONNode* pNode, i32* Unk);
	static i32 calculateMipLevel(u8 Type, u16 Width, u16 Height, u8 Levels, u32 Format);
	static i32 insertStreamingModule(rage::strStreamingModuleMgr* pMgr, rage::strStreamingModule* pModule);
	static i64 updateTimecycleData(u64* _This, TimecycleKeyframeData* pData);
	static bool hasRosPrivilege(u64* _This, i32 Privilege);
	static bool addItemToBasket(CNetworkShoppingMgr* pTransactionMgr, i32* Items);
	static bool request(CHttpRequest* pRequest);
	static bool sendMetric(rage::rlMetric* pMetric, bool Unk);
	static void sendNetworkEvent(rage::netEventMgr* pEventMgr, rage::netGameEvent* pEvent);
	static LPVOID convertThreadToFiber(LPVOID param);
	static FARPROC getProcAddress(HMODULE hModule, LPCSTR lpProcName);
	static bool updateAttributeInt(PresenceData* Data, int ProfileIndex, char* Attribute, u64 Value);
	static HRESULT resizeBuffers(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);
	static HRESULT present(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags);
};
class dynamicFunctionParser {
public:
	dynamicFunctionParser(LPCSTR moduleName, LPCSTR exportName) :
		m_moduleName(moduleName), m_exportName(exportName) 
	{
		if (!m_module) {
			m_module = GetModuleHandleA(m_moduleName);
		}
	}
	FARPROC operator*() {
		if (m_module)
			return GetProcAddress(m_module, m_exportName);
		return nullptr;
	}
private:
	LPCSTR m_moduleName{};
	LPCSTR m_exportName{};
	HMODULE m_module{};
};
class hooking {
	friend struct hooks;
public:
	hooking();
	~hooking();
public:
	void enable();
	void disable();
public:
	MinHook m_minhook;
	detour m_cTaskJumpConstructor;
	detour m_cTaskFallConstructor;
	detour m_runAsyncModuleRequest;
	detour m_hasIntervalElapsed;
	detour m_dispatchEvent;
	detour m_scriptVm;
	detour m_proccessPackedEvents;
	detour m_receiveCloneSync;
	detour m_receiveCloneCreate;
	detour m_canApplyData;
	detour m_findGameMatch;
	detour m_processMatchmakingFind;
	detour m_hasRosPrivilege;
	detour m_calculateMipLevel;
	detour m_insertStreamingModule;
	detour m_updateTimecycleData;
	detour m_addItemToBasket;
	detour m_request;
	detour m_sendMetric;
	detour m_sendNetworkEvent;
	detour m_updateAttributeInt;
	detour m_convertThreadToFiber;
	detour m_getProcAddress;
	hookVFT m_DX;
};
inline SmartPointer<hooking> g_hooking{};