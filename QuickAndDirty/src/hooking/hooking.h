#pragma once
#include "pch/pch.h"
#include "memory/pointers.h"
#include "rage/classes.h"
#include "methods/vmt.h"
#include "methods/detour.h"
#include "shv/dynamic_loader.h"
#include "rage/joaat.h"
#define CALL(hk, ...) g_hooking->m_##hk##.getOg<pointers::types::##hk##>()(__VA_ARGS__);
#define CALL_DECL(hk, ...) g_hooking->m_##hk##.getOg<decltype(&##hk)>()(__VA_ARGS__);
#define RET_CALL(hk, ...) return CALL(hk, __VA_ARGS__);
#define RET_CALL_DECL(hk, ...) return CALL_DECL(hk, __VA_ARGS__);
#define VMT_CALL(vmt, hook, ...) g_hooking->m_##vmt##.getOg<decltype(&hooks::##hook)>(g_##hook##Index)(__VA_ARGS__);

inline u64 g_swapchainSize{ 19 };
inline u64 g_resizeBuffersIndex{ 13 };
inline u64 g_presentIndex{ 8 };
inline std::vector<u32> g_unknownHashes{};
struct hooks {
	static void* cTaskJumpConstructor(u64 _This, u32 Flags);
	static void* cTaskFallConstructor(u64 _This, u32 Flags);
	static void runAsyncModuleRequest(u64* Module);
	static bool hasIntervalElapsed(u32 Timestamp, u32 Interval);
	static u64 gameSkeletonRunUpdate(rage::gameSkeleton* _This, s32 Type);
	static u64 gameSkeletonUpdateBaseRunGroup(rage::gameSkeletonUpdateBase* _This);
	static bool dispatchEvent(u64 _This, rage::netConMgr* pConMgr, rage::netConnection::InFrame* pEvent);
	static LPVOID convertThreadToFiber(LPVOID param);
	static FARPROC getProcAddress(HMODULE hModule, LPCSTR lpProcName);
	static HRESULT resizeBuffers(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);
	static HRESULT present(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags);
};
struct dynamicFunctionParser {
	LPCSTR m_moduleName{};
	LPCSTR m_exportName{};
	HMODULE m_module{ GetModuleHandleA(m_moduleName) };
	FARPROC operator*() {
		if (!m_module) {
			m_module = GetModuleHandleA(m_moduleName);
		}
		return GetProcAddress(m_module, m_exportName);
	}
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
	detour m_gameSkeletonRunUpdate;
	detour m_gameSkeletonUpdateBaseRunGroup;
	detour m_dispatchEvent;
	detour m_convertThreadToFiber;
	detour m_getProcAddress;
	hookVFT m_DX;
};
inline std::unique_ptr<hooking> g_hooking{};