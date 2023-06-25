#include "core/core.h"
#include "util/util.h"
#include "util/clipboard.h"
#include "shv/dynamic_loader.h"
#include "commands/commands.h"
#include "memory/patch.h"
#include <memory/scanner.h>

namespace core {
	scyllaHide g_scyllaHide{};
	namespace thread {
		void create(HMODULE hmodule) {
			g_module = hmodule;
			g_thread = CreateThread(nullptr, NULL, entry, nullptr, NULL, nullptr);
		}
		void loop() {
			while (g_running) {
				std::this_thread::sleep_for(100ms);
			}
		}
		void destroy() {
			CloseHandle(g_thread);
		}
		DWORD entry(LPVOID paramater) {
			g_scyllaHide.load();
			core::create();
			loop();
			core::destroy();
			g_scyllaHide.unload();
			FreeLibraryAndExitThread(g_module, 0);
			return 0;
		}
	}
	void create() {
		g_logger = std::make_unique<logger>("Ovr | Developer (0.00.1)");
		exceptions::initExceptionHandler();
		pointers::scanAll();
		pointers::doPatches();
		while (*pointers::g_gameState != eGameState::Playing) {
			std::this_thread::sleep_for(10ms);
		}
		g_invoker.cache();
		g_fiberPool.create();
		g_renderer = std::make_unique<renderer>();
		g_hooking = std::make_unique<hooking>();
		g_hooking->enable();
		g_manager.add("script", &script::onTick);
		g_manager.add("commands", &commands::onTick);
		engine::createThread(&g_manager);
	}
	void destroy() {
		engine::cleanupThreads();
		g_manager.destroy();
		g_hooking->disable();
		std::this_thread::sleep_for(1s);
		g_renderer.reset();
		g_hooking.reset();
		g_patches.reset();
		commands::g_manager.clear();
		exceptions::uninitExceptionHandler();
		g_logger.reset();
	}
}