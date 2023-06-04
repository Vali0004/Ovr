#include "pch/pch.h"
#include "core/core.h"

BOOL APIENTRY DllMain(HMODULE hmod, DWORD callReason, LPVOID reserved) {
    return thread::create(hmod, callReason);
}