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
extern std::string getCurrentStreamingName();
extern u32 getCurrentStreamingIndex();
constexpr uint64_t g_GB{ 1000 * 1024 * 1024 }; //Use 1000 for one so we catch 'hardware reserved' memory as well
inline int g_extRamMode{};
inline int* g_budgetScale{};
inline int* g_isStereo{};
inline void* allocateStub(u64 size) {
	HMODULE coreRT{ GetModuleHandleA("CoreRT.dll") };
	auto fn{ reinterpret_cast<decltype(&allocateStub)>(GetProcAddress(coreRT, "AllocateStubMemoryImpl")) };
	return fn(size);
}
struct budgeting {
	static void init() {
		g_isStereo = (i32*)allocateStub(4);
		*g_budgetScale = NULL;
		*g_isStereo = TRUE;
		mem loc{ scan("", "84 C0 0F 84 4B 01 00 00 0F B6").sub(0x46) };
		nop(loc.add(0x48).as<void*>(), 6);
		put(loc.add(0xA6).as<i32*>(), (u64)g_budgetScale - loc.add(0xA6).add(4).as<u64>());
		put(loc.add(0xBA).as<i32*>(), (u64)g_isStereo - loc.add(0xBA).add(4).as<u64>());
		call(loc.add(0x101).as<void*>(), bigUpdate);
	}
	static void postInit() {
		MEMORYSTATUSEX msex{};
		msex.dwLength = sizeof(msex);
		GlobalMemoryStatusEx(&msex);
		uint32_t allocatorReservation{};
		if (msex.ullTotalPhys >= 16 * g_GB) {
			allocatorReservation = 0x7FFFFFFF;
			g_extRamMode = 2;
		}
		else if (msex.ullTotalPhys >= 12 * g_GB) {
			allocatorReservation = 0x60000000;
			g_extRamMode = 1;
		}
		//The full code will 100% break 4/4GB systems
		if (g_extRamMode == 0) {
			setGamePhysicalBudget(3 * g_GB);
			return;
		}
		mem grcResourceCachePool{ scan("GRCP", "BA 00 00 05 00 48 8B C8 44 88").add(1) };
		put(grcResourceCachePool.as<u32*>(), 0xA0000);
		put(grcResourceCachePool.add(22).as<u32*>(), 0xA001B);
		//Increase allocator amount
		if (allocatorReservation) {
			put(pointers::g_allocatorAmount, allocatorReservation);
		}
		setGamePhysicalBudget(3 * g_GB);
	}
	static void bigUpdate(int who, int what) {
		*g_budgetScale = what;
		setGamePhysicalBudget(0);
		pointers::g_updateVideoMemoryBar(0);
	}
	static void setGamePhysicalBudget(u64 budget) {
		static u64 baseBudget{};
		if (budget == 0) {
			budget = baseBudget;
		}
		else {
			baseBudget = budget;
		}
		float multiplier{ getBudgetMultiplier() };
		//This is designed to fix the logic error with low/high/veryhigh
		//The logic error/issue is R* seems to thought in a few cases that the texture setting flag is mapped to
		// normal, high, very high, and not unused, normal, high/very high.
		// This creates the issue where very high is just high with the hi texture flag enabled.
		//This will fix low and normal actually being low and normal
		for (i32 i{}; i != 80; i += 4) {
			u64 value{ static_cast<u64>(budget * multiplier) };
			pointers::g_vramLocation[i + 3] = value;
			pointers::g_vramLocation[i + 2] = value;
			pointers::g_vramLocation[i + 1] = static_cast<u64>(value / 1.5f);
			pointers::g_vramLocation[i] = static_cast<u64>(value / 2.f);
		}
	}
	static float getBudgetMultiplier() {
		return (*g_budgetScale / 12.f) + 1.f;
	}
};
inline void accessTlsStorageFromAnotherThread(u32 hash, std::function<void(rage::tlsContext*)> callback) {
	try {
		rage::tlsContext* threadStorage{ rage::tlsContext::get() };
		GtaThread* thread{ util::classes::getGtaThread(hash) };
		rage::scrThread* oThread{ threadStorage->m_script_thread };
		rage::sysMemAllocator* oSysMemAllocater{ threadStorage->m_allocator };
		rage::sysMemAllocator* oTlsSysMemAllocater{ threadStorage->m_tls_entry };
		rage::sysMemAllocator* oUnkTlsSysMemAllocater{ threadStorage->m_unk_allocator };
		threadStorage->m_script_thread = thread;
		threadStorage->m_allocator = threadStorage->m_tls_entry;
		threadStorage->m_tls_entry = threadStorage->m_allocator;
		threadStorage->m_unk_allocator = threadStorage->m_tls_entry;
		threadStorage->m_is_script_thread_active = true;
		callback(threadStorage);
		threadStorage->m_script_thread = oThread;
		threadStorage->m_allocator = oSysMemAllocater;
		threadStorage->m_tls_entry = oTlsSysMemAllocater;
		threadStorage->m_unk_allocator = oUnkTlsSysMemAllocater;
		threadStorage->m_is_script_thread_active = oThread->m_serialised.m_state == rage::eThreadState::running;
	}
	catch (std::runtime_error& err) {
		LOG_DEBUG("Runtime error {} in {}", err.what(), __FUNCTION__);
	}
	catch (std::exception& ex) {
		LOG_DEBUG("Exception {} in {}", ex.what(), __FUNCTION__);
	}
	catch (...) {
		LOG_DEBUG("Unknown exception in {}", __FUNCTION__);
	}
}
struct requestData {
	std::string protocol{};
	std::string baseUrl{};
	std::string endpoint{};
	std::string url{};
	std::string response{};
	std::string contentLength{};
	std::vector<std::string> headers{};
	void addHeadersFromString(std::string data) {
		std::string line{};
		std::istringstream stream{ data };
		while (std::getline(stream, line)) {
			if (line.empty()) {
				break;
			}
			headers.push_back(line);
		}
	}
};
inline std::vector<requestData> g_requests{};
inline u64 g_selectedRequest{ (u64)-1 };
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
	static u64 getAvailableMemoryForStreamer(u64* _This);
	static u64 settingsVramTex(u64* _This, i32 Quality, u64* pSettings);
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
	detour m_getAvailableMemoryForStreamer;
	detour m_settingsVramTex;
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