#pragma once
#include "pch/pch.h"
#include "rage/enums.h"
#include "renderer/renderer.h"
#include "renderer/dxtk/dxtexture2d_wrapper.h"
typedef void(*TKeyboardFn)(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);
typedef void(*PresentCallback)(void*);

namespace shv {
	namespace exports {
		extern void scriptRegister(HMODULE module, void(*LP_SCRIPT_MAIN)());
		extern void scriptRegisterAdditionalThread(HMODULE module, void(*LP_SCRIPT_MAIN)());
		extern void scriptUnregister(HMODULE module);
		extern void scriptUnregisterDepricated(void(*LP_SCRIPT_MAIN)());
		extern void nativeInit(uint64_t hash);
		extern void nativePush64(uint64_t val);
		extern PUINT64 nativeCall();
		extern PUINT64 getGlobalPtr(int index);
		extern int createTexture(const char* fileName);
		extern void drawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a);
		extern void scriptWait(DWORD waitTime);
		extern void keyboardHandlerRegister(TKeyboardFn function);
		extern void keyboardHandlerUnregister(TKeyboardFn function);
		extern eGameVersion getGameVersion();
		extern BYTE* getScriptHandleBaseAddress(int handle);
		extern void presentCallbackUnregister(PresentCallback cb);
		extern void presentCallbackRegister(PresentCallback cb);
		extern int worldGetAllVehicles(int* arr, int arrSize);
		extern int worldGetAllPeds(int* arr, int arrSize);
		extern int worldGetAllObjects(int* arr, int arrSize);
		extern int worldGetAllPickups(int* arr, int arrSize);
	}
	extern void onPresent(renderer* renderer);
	extern void wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	extern void onRendererReset();
	inline uint64_t g_hash{};
	inline std::set<TKeyboardFn> g_keyboardFunctions{};
	inline std::set<PresentCallback> g_presentCallbacks{};
	inline std::unordered_map<int, std::string> g_createTextureArray{};
	inline std::vector<scriptTex> g_drawTextureArray{};
	inline std::vector<reloadTex> g_reloadArray{};
	inline int g_drawTextureArrayIndex{};
}