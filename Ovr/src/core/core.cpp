#include "core/core.h"
#include "util/util.h"
#include "util/clipboard.h"
#include "shv/dynamic_loader.h"
#include "commands/commands.h"
#include "memory/patch.h"
#include "memory/scanner.h"
#include "util/transaction.h"
#include "rage/script/loader.h"
#include "fiber/dx_manager.h"
#include <util/exploit.h>

namespace core {
	namespace thread {
		void create(HMODULE hmodule) {
			g_module = hmodule;
			g_thread = CreateThread(nullptr, NULL, entry, nullptr, NULL, nullptr);
		}
		void loop() {
			while (g_running) {
				if (GetAsyncKeyState(VK_F12)) {
					g_running = false;
				}
				std::this_thread::sleep_for(100ms);
			}
		}
		void destroy() {
			CloseHandle(g_thread);
		}
		DWORD entry(LPVOID paramater) {
			while (!FindWindowA("grcWindow", nullptr) || !GetModuleHandleA("socialclub.dll")) {
				std::this_thread::sleep_for(500ms);
			}
			core::create();
			loop();
			core::destroy();
			FreeLibraryAndExitThread(g_module, 0);
			return 0;
		}
	}
	void create() {
		g_logger = MakeSmartPointer<logger>(BRAND " | Developer (0.00.1, b3311)"); //I really need to keep track of this...
		g_scyllaHide = MakeSmartPointer<scyllaHide>();
		if (g_scyllaHide->getModule()) {
			LOG_DEBUG("ScyllaHide loaded.");
		}
		shv::g_shvLoader = MakeSmartPointer<shv::shvLoader>();
		if (shv::g_shvLoader->getModule()) {
			LOG_DEBUG("SHV module loaded.");
		}
		exceptions::initExceptionHandler();
		pointers::scanAll();
		LOG(Info, "{}/{} pointers found. ({} failed)", g_foundSigCount, g_totalSigCount, g_failedSigCount);
		util::game::commands::intialize();
		std::this_thread::sleep_for(100ms);
		pointers::doPatches();
		g_invoker.cache();
		g_pool.create();
		g_renderer = MakeSmartPointer<renderer>();
		g_hooking = MakeSmartPointer<hooking>();
		g_hooking->enable();
		g_manager.add("script", &script::onTick);
		g_manager.add("commands", &commands::onTick);
		g_manager.add("playerManager", &util::network::manager::onTick);
		g_manager.add("commandStream", &commands::engine::commandStreamTick);
		g_manager.add("statistics", [] {
			while (true) {
				g_statistics.reset();
				fiber::current()->sleep(std::chrono::milliseconds(g_statistics.getLastFrameTime()));
			}
		});
		g_dxManager.add("renderer", &renderer::onTick);
		util::playSound("InjectionSound");
		while (*pointers::g_loadingScreenState != eLoadingScreenState::Finished) {
			std::this_thread::sleep_for(100ms);
		}
		rage::ysc::g_loader = MakeSmartPointer<rage::ysc::loader>();
		engine::createThread(&g_manager);
	}
	void destroy() {
		std::this_thread::sleep_for(500ms);
		engine::cleanupThreads();
		g_manager.destroy();
		g_hooking->disable();
		std::this_thread::sleep_for(1s);
		g_renderer.reset();
		g_hooking.reset();
		rage::ysc::g_loader.reset();
		shv::g_asiLoader.clear();
		shv::g_shvLoader.reset();
		g_patches.reset();
		commands::g_manager.clear();
		exceptions::uninitExceptionHandler();
		g_arxPatches.clear();
		budgeting::restore();
		g_logger.reset();
		g_scyllaHide.reset();
	}
}