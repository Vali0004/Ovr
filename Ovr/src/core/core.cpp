#include "core/core.h"
#include "util/util.h"
#include "util/clipboard.h"
#include "shv/dynamic_loader.h"
#include "commands/commands.h"
#include "memory/patch.h"
#include <memory/scanner.h>
#include <Mmsystem.h>

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
			core::create();
			loop();
			core::destroy();
			FreeLibraryAndExitThread(g_module, 0);
			return 0;
		}
	}
	void create() {
		g_logger = MakeSmartPointer<logger>("Ovr | Developer (0.00.1, b1503)");
		g_scyllaHide = MakeSmartPointer<scyllaHide>();
		if (g_scyllaHide->getModule()) {
			LOG(Debug, "ScyllaHide loaded.");
		}
		//shv::g_shvLoader = MakeSmartPointer<shv::shvLoader>();
		//if (shv::g_shvLoader->getModule())
		//	LOG(Debug, "SHV module loaded.");
		exceptions::initExceptionHandler();
		pointers::scanAll();
		while (*pointers::g_loadingScreenState != eLoadingScreenState::Finished) {
			std::this_thread::sleep_for(10ms);
		}
		pointers::doPatches();
		g_invoker.cache();
		g_fiberPool.create();
		g_renderer = MakeSmartPointer<renderer>();
		g_hooking = MakeSmartPointer<hooking>();
		g_hooking->enable();
		g_manager.add("script", &script::onTick);
		g_manager.add("commands", &commands::onTick);
		g_manager.add("playerManager", &util::network::manager::onTick);
		engine::createThread(&g_manager);
		fs::path path{ std::getenv("appdata") };
		path /= BRAND"\\Sounds\\injection_sound.wav";
		sndPlaySoundA(path.string().c_str(), SND_FILENAME | SND_ASYNC);
	}
	void destroy() {
		g_fiberPool.add(&commands::features::uninit);
		//We'd like ped flags and other things to be reset because of cases like no-clip
		std::this_thread::sleep_for(200ms);
		engine::cleanupThreads();
		g_manager.destroy();
		g_hooking->disable();
		std::this_thread::sleep_for(1s);
		g_renderer.reset();
		g_hooking.reset();
		shv::g_asiLoader.clear();
		shv::g_shvLoader.reset();
		g_patches.reset();
		commands::g_manager.clear();
		exceptions::uninitExceptionHandler();
		g_logger.reset();
		g_scyllaHide.reset();
	}
}