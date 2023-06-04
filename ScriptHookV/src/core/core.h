#pragma once
#include "pch/pch.h"

namespace core {
	extern void initialize();
	extern void loop();
	extern void uninitialize(HMODULE hmod);
}
namespace thread {
	extern DWORD entry(LPVOID param);
	extern BOOL create(HMODULE hmod, DWORD callReason);
}