#include "pch/pch.h"
#include "commands.h"
#include "fiber/fiber.h"
#include "util/util.h"
#include "util/clipboard.h"
#include "features.h"
#include "gui/gui.h"
#include "renderer/renderer.h"

namespace commands {
	void copyText(stringCommand* command) {
		util::clipboard clipboard{ command->get_context() };
		clipboard.set();
	}
	void copyScString(stringCommand* command) {
		g_engine.primitiveExecute("copyText {}", util::network::socialclub::getString(command->get_string().c_str()));
	}
	void printCliboard(actionCommand* command) {
		util::clipboard clipboard{};
		clipboard.get();
		LOG_DIRECT(White, "Clipboard", "{}", clipboard.str());
	}
	void useDirectMatchResult(toggleCommand* command) {
		g_engine.m_useDirectMatchResults = command->get(0).toggle;
	}
	void autoCompleteCommands(toggleCommand* command) {
		g_engine.m_autoComplete = command->get(0).toggle;
	}
	void useFirstCommandOnMultipleResults(toggleCommand* command) {
		g_engine.m_useFirstResultOnTooManyResults = command->get(0).toggle;
	}
	void clearCommandBoxOnEnter(toggleCommand* command) {
		gui::g_box.m_clearCommandBoxOnEnter = command->get(0).toggle;
	}
	void print(stringCommand* command) {
		LOG(Info, command->get_context());
	}
	void forceQuitToSp(actionCommand* command) {
		NETWORK::SHUTDOWN_AND_LOAD_MOST_RECENT_SAVE();
	}
	void restartGame(actionCommand* command) {
		MISC::RESTART_GAME();
	}
	void resetState(actionCommand* command) {
		for (auto& entry : g_manager.getCommands()) {
			auto& cmd{ entry.second };
			switch (cmd->m_type) {
			case eCommandType::ToggleIntCommand:
			case eCommandType::ToggleFloatCommand:
			case eCommandType::ToggleCommand: {
				cmd->get(0).toggle = false;
			} break;
			}
		}
		features::onInit();
		"useDirectMatchResults"_TC->get(0).toggle = true;
		"autoCompleteCommands"_TC->get(0).toggle = true;
		"clearCommandBoxOnEnter"_TC->get(0).toggle = true;
	}
	void nut(actionCommand* command) {
		util::playSound("NUT");
	}
	void init() {
		g_manager.add(stringCommand("copyText", "Copy Text", "Copies text to clipboard", copyText));
		g_manager.add(stringCommand("copyScString", "Copy Socialclub String", "Copies a string from socialclub.dll to clipboard", copyScString));
		g_manager.add(actionCommand("printClipboard", "Print Clipboard", "Prints your clipboard to log", printCliboard));
		g_manager.add(toggleCommand("useDirectMatchResults", "Use Direct Match Results", "When an command is a direct match, it will use that when possible", useDirectMatchResult));
		g_manager.add(toggleCommand("autoCompleteCommands", "Auto Complete Commands", "When a command is still being typed and there is no other results, it will auto complete the command", autoCompleteCommands));
		g_manager.add(toggleCommand("useFirstCommandOnMultipleResults", "Use The First Command On Multiple Results", "When an command has multiple results, it will use the closet resembling command", useFirstCommandOnMultipleResults));
		g_manager.add(toggleCommand("clearCommandBoxOnEnter", "Clear Command Box On Enter", clearCommandBoxOnEnter));
		g_manager.add(stringCommand("print", "Print", "Prints a string", print));
		g_manager.add(actionCommand("forceQuitToSp", "Force Quit To Story Mode", "Forcefully quits to SP", forceQuitToSp));
		g_manager.add(actionCommand("restartGame", "Restart Game", "Restarts your game", restartGame));
		g_manager.add(actionCommand("resetState", "Reset Menu State", "Resets all commands and GUI elements", resetState));
		g_manager.add(actionCommand("nut", "da nut command (bess command)", nut));
		features::init();
		g_manager.init();
	}
	void onInit() {
		features::onInit();
		"useDirectMatchResults"_TC->get(0).toggle = true;
		"autoCompleteCommands"_TC->get(0).toggle = true;
		"clearCommandBoxOnEnter"_TC->get(0).toggle = true;
	}
	void onTick() {
		init();
		onInit();
		while (true) {
			features::onTick();
			g_manager.tick();
			fiber::current()->sleep();
		}
	}
}