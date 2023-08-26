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
#include "util/budgeting_fix.h"
#define CALL(hk, ...) g_hooking->m_##hk##.getOg<pointers::types::##hk##>()(__VA_ARGS__)
#define CALL_DECL(hk, ...) g_hooking->m_##hk##.getOg<decltype(&##hk)>()(__VA_ARGS__)
#define RET_CALL(hk, ...) return CALL(hk, __VA_ARGS__);
#define RET_CALL_DECL(hk, ...) return CALL_DECL(hk, __VA_ARGS__);
#define VMT_CALL(vmt, hook, ...) g_hooking->m_##vmt##.getOg<decltype(&hooks::##hook)>(g_##hook##Index)(__VA_ARGS__)

inline u64 g_swapchainSize{ 19 };
inline u64 g_resizeBuffersIndex{ 13 };
inline u64 g_presentIndex{ 8 };
inline u64 g_updateAttributeIntIndex{ 1 };
extern std::string getCurrentStreamingName();
extern u32 getCurrentStreamingIndex();
class threadStorageAccessor {
public:
	void tick() { //Handle cacher
		m_threadLocalStorage = rage::tlsContext::get();
		m_thread = m_threadLocalStorage->m_script_thread;
		m_allocator = m_threadLocalStorage->m_allocator;
		if (!m_allocator) {
			m_allocator = m_threadLocalStorage->m_tls_entry;
		}
		if (!m_allocator) {
			m_allocator = m_threadLocalStorage->m_unk_allocator;
		}
	}
	void access(std::function<void()> callback) {
		rage::sysMemAllocator* allocatorStorage{ m_threadLocalStorage->m_allocator };
		rage::scrThread* threadStorage{ m_threadLocalStorage->m_script_thread };
		m_threadLocalStorage->m_allocator = m_allocator;
		m_threadLocalStorage->m_script_thread = m_thread;
		m_threadLocalStorage->m_is_script_thread_active = true;
		std::invoke(callback);
		m_threadLocalStorage->m_allocator = allocatorStorage;
		m_threadLocalStorage->m_script_thread = threadStorage;
		m_threadLocalStorage->m_is_script_thread_active = threadStorage->m_serialised.m_state == rage::eThreadState::running;
	}
private:
	rage::tlsContext* m_threadLocalStorage{};
	rage::scrThread* m_thread{};
	rage::sysMemAllocator* m_allocator{};
};
inline threadStorageAccessor g_threadStorageAccessor{};
struct hooks {
	static void* cTaskJumpConstructor(u64 _This, u32 Flags);
	static void* cTaskFallConstructor(u64 _This, u32 Flags);
	static void runAsyncModuleRequest(u64* Module);
	static bool dispatchEvent(u64 _This, rage::netConnectionManager* pConMgr, rage::netConnection::InFrame* pEvent);
	static rage::eThreadState scriptVm(rage::scrValue* stack, rage::scrValue** globals, rage::scrProgram* pt, rage::scrThread::Serialised* ser);
	static void proccessPackedEvents(rage::netEventMgr* pEventMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, u16 Id, i32 Index, i32 HandledBitset, i32 BufferSize, rage::datBitBuffer* Buffer);
	static eAckCode receiveCloneSync(CNetworkObjectMgr* pObjMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, eNetObjectType ObjectType, u16 ObjectId, rage::datBitBuffer* Buffer, u16 Unknown, u32 Timestamp);
	static bool receiveCloneCreate(CNetworkObjectMgr* pObjMgr, CNetGamePlayer* Sender, CNetGamePlayer* Receiver, eNetObjectType ObjectType, i32 ObjectId, i32 ObjectFlag, rage::datBitBuffer* Buffer, i32 Timestamp);
	static bool canApplyData(rage::netSyncTree* pSyncTree, rage::netObject* pObject);
	static bool findGameMatch(i32 ProfileIndex, i32 AvailableSlots, NetworkGameFilterMatchmakingComponent* pFilter, i32 Count, rage::rlSessionInfo* pSessions, i32* OutputSize, rage::rlTaskStatus* pStatus);
	static bool processMatchmakingFind(u64* _This, u64* Unused, rage::JSONNode* pNode, i32* Unk);
	static i32 calculateMipLevel(u8 Type, u16 Width, u16 Height, u8 Levels, u32 Format);
	static i32 insertStreamingModule(rage::strStreamingModuleMgr* pMgr, rage::strStreamingModule* pModule);
	static i64 updateTimecycleData(u64* _This, TimecycleKeyframeData* pData);
	static void* allocateReliable(rage::netConnection* pCon, i32 RequiredMemory);
	static bool hasRosPrivilege(u64* _This, i32 Privilege);
	static u64 writePlayerGameStateDataNode(rage::netObject* pObject, CPlayerGameStateDataNode* pNode);
	static bool getNewsStory(CNetworkSCNewsStory* pStory);
	static bool presencePostRawMessage(int localGamerindex, const rage::rlGamerHandle* recipients, unsigned int numRecipients, const char* message, unsigned int ttlSeconds);
	static u64 getAvailableMemoryForStreamer(u64* _This);
	static u64 settingsVramTex(u64* _This, i32 Quality, u64* pSettings);
	static bool hasGameBeenAltered();
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
	detour m_allocateReliable;
	detour m_writePlayerGameStateDataNode;
	detour m_getNewsStory;
	detour m_presencePostRawMessage;
	detour m_getAvailableMemoryForStreamer;
	detour m_settingsVramTex;
	detour m_hasGameBeenAltered;
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