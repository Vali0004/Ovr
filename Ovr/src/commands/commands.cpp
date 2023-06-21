#include "pch/pch.h"
#include "commands.h"
#include "fiber/fiber.h"
#include "util/util.h"
#include "util/clipboard.h"
#include "features.h"

namespace commands {
	void copyText(variadicCommand* command) {
		util::clipboard clipboard{ command->get(0).string };
		clipboard.set();
	}
	void copyScString(variadicCommand* command) {
		g_engine.primitiveExecute("copyText {}", util::network::socialclub::getString(command->get(0).string));
	}
	void printCliboard(actionCommand* command) {
		util::clipboard clipboard{};
		clipboard.get();
		LOG_DIRECT(White, "Clipboard", "{}", clipboard.str());
	}
	void onTick() {
		g_manager.add(variadicCommand("copyText", "Copy Text", "Copies text to clipboard", { { eValueType::String } }, commands::copyText, false));
		g_manager.add(variadicCommand("copyScString", "Copy Socialclub String", "Copies a string from socialclub.dll to clipboard", { { eValueType::String } }, commands::copyScString, false));
		g_manager.add(actionCommand("printClipboard", "Print Clipboard", "Prints your clipboard to log", commands::printCliboard));
		features::init();
		g_manager.init();
		features::onInit();
		while (true) {
			features::onTick();
			g_manager.tick();
			fiber::current()->sleep();
		}
	}
}