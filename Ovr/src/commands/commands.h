#pragma once
#include "manager/manager.h"
#include "engine/engine.h"

namespace commands {
	extern void copyText(stringCommand* command);
	extern void copyScString(stringCommand* command);
	extern void printCliboard(actionCommand* command);
	extern void useDirectMatchResult(toggleCommand* command);
	extern void autoCompleteCommands(toggleCommand* command);
	extern void useFirstCommandOnMultipleResults(toggleCommand* command);
	extern void clearCommandBoxOnEnter(toggleCommand* command);
	extern void print(stringCommand* command);
	extern void forceQuitToSp(actionCommand* command);
	extern void resetState(actionCommand* command);
	extern void nut(actionCommand* command);
	extern void init();
	extern void onInit();
	extern void onTick();
}