#pragma once
#include "pch/pch.h"
#include "core/core.h"
#define DESTRINGIFY(v) x##v
#define EXPORT __declspec(dllexport)
typedef void(*TKeyboardFn)(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);
typedef void(*PresentCallback)(void*);

enum eGameVersion : int {
	VER_1_0_335_2_STEAM,
	VER_1_0_335_2_NOSTEAM,
	VER_1_0_350_1_STEAM,
	VER_1_0_350_2_NOSTEAM,
	VER_1_0_372_2_STEAM,
	VER_1_0_372_2_NOSTEAM,
	VER_1_0_393_2_STEAM,
	VER_1_0_393_2_NOSTEAM,
	VER_1_0_393_4_STEAM,
	VER_1_0_393_4_NOSTEAM,
	VER_1_0_463_1_STEAM,
	VER_1_0_463_1_NOSTEAM,
	VER_1_0_505_2_STEAM,
	VER_1_0_505_2_NOSTEAM,
	VER_1_0_573_1_STEAM,
	VER_1_0_573_1_NOSTEAM,
	VER_1_0_617_1_STEAM,
	VER_1_0_617_1_NOSTEAM,
	VER_1_0_678_1_STEAM,
	VER_1_0_678_1_NOSTEAM,
	VER_1_0_757_2_STEAM,
	VER_1_0_757_2_NOSTEAM,
	VER_1_0_757_4_STEAM,
	VER_1_0_757_4_NOSTEAM,
	VER_1_0_791_2_STEAM,
	VER_1_0_791_2_NOSTEAM,
	VER_1_0_877_1_STEAM,
	VER_1_0_877_1_NOSTEAM,
	VER_1_0_944_2_STEAM,
	VER_1_0_944_2_NOSTEAM,
	VER_1_0_1011_1_STEAM,
	VER_1_0_1011_1_NOSTEAM,
	VER_1_0_1032_1_STEAM,
	VER_1_0_1032_1_NOSTEAM,
	VER_1_0_1103_2_STEAM,
	VER_1_0_1103_2_NOSTEAM,
	VER_1_0_1180_2_STEAM,
	VER_1_0_1180_2_NOSTEAM,
	VER_1_0_1290_1_STEAM,
	VER_1_0_1290_1_NOSTEAM,
	VER_1_0_1365_1_STEAM,
	VER_1_0_1365_1_NOSTEAM,
	VER_1_0_1493_0_STEAM,
	VER_1_0_1493_0_NOSTEAM,
	VER_1_0_1493_1_STEAM,
	VER_1_0_1493_1_NOSTEAM,
	VER_1_0_1604_0_STEAM,
	VER_1_0_1604_0_NOSTEAM,
	VER_1_0_1604_1_STEAM,
	VER_1_0_1604_1_NOSTEAM,
	VER_1_0_1737_0_STEAM,
	VER_1_0_1737_0_NOSTEAM,
	VER_1_0_1737_6_STEAM,
	VER_1_0_1737_6_NOSTEAM,
	VER_1_0_1868_0_STEAM,
	VER_1_0_1868_0_NOSTEAM,
	VER_1_0_1868_1_STEAM,
	VER_1_0_1868_1_NOSTEAM,
	VER_1_0_1868_4_EPIC,
	VER_1_0_2060_0_STEAM,
	VER_1_0_2060_0_NOSTEAM,
	VER_1_0_2060_1_STEAM,
	VER_1_0_2060_1_NOSTEAM,
	VER_1_0_2189_0_STEAM,
	VER_1_0_2189_0_NOSTEAM,
	VER_1_0_2545_STEAM,
	VER_1_0_2545_0_NOSTEAM,
	VER_1_0_2612_STEAM,
	VER_1_0_2612_0_NOSTEAM,
	VER_SIZE,
	VER_UNK = -1
};

EXPORT void scriptRegister(HMODULE module, void(*LP_SCRIPT_MAIN)()) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&scriptRegister))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(module, LP_SCRIPT_MAIN);
}
EXPORT void scriptRegisterAdditionalThread(HMODULE module, void(*LP_SCRIPT_MAIN)()) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&scriptRegisterAdditionalThread))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(module, LP_SCRIPT_MAIN);
}
EXPORT void scriptUnregister(HMODULE module) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (void(*)(HMODULE module))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(module);
}
EXPORT void scriptUnregister(void(*LP_SCRIPT_MAIN)()) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (void(*)(void(*LP_SCRIPT_MAIN)()))GetProcAddress(global::g_exportModule, __FUNCTION__"Depricated") };
	if (!fn)
		return;
	return fn(LP_SCRIPT_MAIN);
}
EXPORT void nativeInit(uint64_t hash) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&nativeInit))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(hash);
}
EXPORT void nativePush64(uint64_t val) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&nativePush64))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(val);
}
EXPORT PUINT64 nativeCall() {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&nativeCall))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn();
}
EXPORT PUINT64 getGlobalPtr(int index) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&getGlobalPtr))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(index);
}
EXPORT int registerRawStreamingFile(const std::string& fileName, const std::string& registerAs) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&registerRawStreamingFile))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(fileName, registerAs);
}
EXPORT int createTexture(const char* fileName) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&createTexture))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(fileName);
}
EXPORT void drawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&drawTexture))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(id, index, level, time, sizeX, sizeY, centerX, centerY, posX, posY, rotation, screenHeightScaleFactor, r, g, b, a);
}
EXPORT void scriptWait(unsigned long waitTime) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&scriptWait))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(waitTime);
}
EXPORT void keyboardHandlerRegister(TKeyboardFn function) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&keyboardHandlerRegister))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(function);
}
EXPORT void keyboardHandlerUnregister(TKeyboardFn function) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&keyboardHandlerUnregister))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return;
	return fn(function);
}
EXPORT eGameVersion getGameVersion() {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&getGameVersion))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return eGameVersion::VER_UNK;
	return fn();
}
EXPORT BYTE* getScriptHandleBaseAddress(int handle) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&getScriptHandleBaseAddress))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(handle);
}
EXPORT void presentCallbackUnregister(PresentCallback cb) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&presentCallbackUnregister))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	return fn(cb);
}
EXPORT void presentCallbackRegister(PresentCallback cb) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&presentCallbackRegister))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	return fn(cb);
}
EXPORT int worldGetAllVehicles(int* arr, int arrSize) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&worldGetAllVehicles))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(arr, arrSize);
}
EXPORT int worldGetAllPeds(int* arr, int arrSize) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&worldGetAllPeds))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(arr, arrSize);
}
EXPORT int worldGetAllObjects(int* arr, int arrSize) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&worldGetAllObjects))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(arr, arrSize);
}
EXPORT int worldGetAllPickups(int* arr, int arrSize) {
	printf("%s called\n", __FUNCTION__);
	auto fn{ (decltype(&worldGetAllPickups))GetProcAddress(global::g_exportModule, __FUNCTION__) };
	if (!fn)
		return NULL;
	return fn(arr, arrSize);
}