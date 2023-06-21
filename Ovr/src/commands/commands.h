#pragma once
#include "manager/manager.h"
#include "engine/engine.h"

namespace commands {
	extern void copyText(variadicCommand* command);
	extern void copyScString(variadicCommand* command);
	extern void printCliboard(actionCommand* command);
	extern void onTick();
}