#include "shv/scripthookv.h"
#include "util/util.h"
#include "util/global.h"
#include "fiber/manager.h"
#include "rage/commands/invoker/invoker.h"

namespace shv {
	namespace exports {
		void scriptRegister(HMODULE module, void(*LP_SCRIPT_MAIN)()) {
			static char name[256];
			if (GetModuleFileNameA(module, name, 256)) {
				if (strstr(name, "Menyoo")) {
					uint64_t base{ uint64_t(module) };
					uint64_t address{ base + 0x2CDC06 };
					std::vector<uint8_t> patch{ 0x90, 9 };
					DWORD oldProtect{};
					VirtualProtect((void*)address, patch.size(), PAGE_EXECUTE_READWRITE, &oldProtect);
					memcpy((void*)address, patch.data(), patch.size());
					VirtualProtect((void*)address, patch.size(), oldProtect, &oldProtect);
				}
			}
			g_manager.add(std::format("0x{:X}:0x{:X}", (uint64_t)module, (uint64_t)LP_SCRIPT_MAIN).c_str(), LP_SCRIPT_MAIN);
		}
		void scriptRegisterAdditionalThread(HMODULE module, void(*LP_SCRIPT_MAIN)()) {
			g_manager.add(std::format("0x{:X}:0x{:X}", (uint64_t)module, (uint64_t)LP_SCRIPT_MAIN).c_str(), LP_SCRIPT_MAIN);
		}
		void scriptUnregister(HMODULE module) {
			g_manager.removeBase(std::format("0x{:X}", (uint64_t)module).c_str());
		}
		void scriptUnregisterDepricated(void(*LP_SCRIPT_MAIN)()) {
			g_manager.removeBase(std::format("0x{:X}", (uint64_t)LP_SCRIPT_MAIN).c_str());
		}
		void nativeInit(uint64_t hash) {
			g_hash = hash;
			g_invoker.begin();
		}
		void nativePush64(uint64_t val) {
			g_invoker.push(static_cast<uint64_t&&>(val));
		}
		PUINT64 nativeCall() {
			g_invoker.end(g_hash);
			return g_invoker.getRetPointer<uint64_t>();
		}
		PUINT64 getGlobalPtr(int index) {
			return PUINT64(global(index).value());
		}
		int createTexture(const char* fileName) {
			return g_renderer->createTexture(fileName);
		}
		void drawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a) {
			g_renderer->drawTexture(id, index, level, time, sizeX, sizeY, centerX, centerY, posX, posY, rotation, screenHeightScaleFactor, r, g, b, a);
		}
		void scriptWait(DWORD waitTime) {
			fiber::current()->sleep(std::chrono::milliseconds(waitTime));
		}
		void keyboardHandlerRegister(TKeyboardFn function) {
			g_keyboardFunctions.insert(function);
		}
		void keyboardHandlerUnregister(TKeyboardFn function) {
			g_keyboardFunctions.erase(function);
		}
		eGameVersion getGameVersion() {
			return GetModuleHandleA("steam_api64.dll") ? VER_1_0_1493_1_STEAM : VER_1_0_1493_1_NOSTEAM;
		}
		BYTE* getScriptHandleBaseAddress(int handle) {
			return (BYTE*)pointers::g_handleToPointer(handle);
		}
		void presentCallbackUnregister(PresentCallback cb) {
			g_presentCallbacks.insert(cb);
		}
		void presentCallbackRegister(PresentCallback cb) {
			g_presentCallbacks.erase(cb);
		}
		int worldGetAllVehicles(int* arr, int arrSize) {
			return util::getPoolObjects<CVehicleInterface>(0, arr, arrSize);
		}
		int worldGetAllPeds(int* arr, int arrSize) {
			return util::getPoolObjects<CPedInterface>(1, arr, arrSize);
		}
		int worldGetAllObjects(int* arr, int arrSize) {
			return util::getPoolObjects<CObjectInterface>(2, arr, arrSize);
		}
		int worldGetAllPickups(int* arr, int arrSize) {
			return util::getPoolObjects<CPickupInterface>(3, arr, arrSize);
		}
	}
	void onPresent(renderer* renderer) {
		renderer->createTextures();
		if (!g_drawTextureArray.empty()) {
			renderer->m_spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_BackToFront, renderer->m_commonState->NonPremultiplied());
			renderer->m_stateSaved = renderer->m_stateSaver->save(renderer->m_context.Get());
			for (auto& t : g_drawTextureArray) {
				if (t.tex.m_enabled)
					t.tex.draw(*renderer->m_spriteBatch);
			}
			renderer->m_spriteBatch->End();
			if (!renderer->m_stateSaved)
				renderer->m_stateSaver->restore();
		}
		for (auto& cb : g_presentCallbacks) {
			renderer->m_stateSaved = renderer->m_stateSaver->save(renderer->m_context.Get());
			cb(renderer->m_swapchain.Get());
			if (!renderer->m_stateSaved)
				renderer->m_stateSaver->restore();
		}
	}
	void onRendererReset() {
		if (!g_createTextureArray.empty())
			g_createTextureArray.clear();
		if (!g_drawTextureArray.empty()) {
			for (auto& val : g_drawTextureArray) {
				val.tex.m_texture->Release();
				val.tex.m_resource->Release();
				val.tex.m_view->Release();
			}
			g_drawTextureArray.clear();
		}
		if (!g_reloadArray.empty())
			g_reloadArray.clear();
	}
}