#include "hooking/hooking.h"
#include "rage/joaat.h"
#include "shv/scripthookv.h"

FARPROC hooks::getProcAddress(HMODULE hModule, LPCSTR lpProcName) {
	if (hModule == reinterpret_cast<HMODULE>(static_cast<u64>(BRAND"__dummy_module"_joaat))) {
		printf("[SHV Redirected Export Engine]: Export for %s requested\n", lpProcName);
		switch (rage::joaat(lpProcName)) {
		case "createTexture"_joaat: { return FARPROC(&shv::exports::createTexture); } break;
		case "drawTexture"_joaat: { return FARPROC(&shv::exports::drawTexture); } break;
		case "getGameVersion"_joaat: { return FARPROC(&shv::exports::getGameVersion); } break;
		case "getGlobalPtr"_joaat: { return FARPROC(&shv::exports::getGlobalPtr); } break;
		case "getScriptHandleBaseAddress"_joaat: { return FARPROC(&shv::exports::getScriptHandleBaseAddress); } break;
		case "keyboardHandlerRegister"_joaat: { return FARPROC(&shv::exports::keyboardHandlerRegister); } break;
		case "keyboardHandlerUnregister"_joaat: { return FARPROC(&shv::exports::keyboardHandlerUnregister); } break;
		case "nativeCall"_joaat: { return FARPROC(&shv::exports::nativeCall); } break;
		case "nativeInit"_joaat: { return FARPROC(&shv::exports::nativeInit); } break;
		case "nativePush64"_joaat: { return FARPROC(&shv::exports::nativePush64); } break;
		case "presentCallbackRegister"_joaat: { return FARPROC(&shv::exports::presentCallbackRegister); } break;
		case "presentCallbackUnregister"_joaat: { return FARPROC(&shv::exports::presentCallbackUnregister); } break;
		case "scriptRegister"_joaat: { return FARPROC(&shv::exports::scriptRegister); } break;
		case "scriptRegisterAdditionalThread"_joaat: { return FARPROC(&shv::exports::scriptRegisterAdditionalThread); } break;
		case "scriptUnregister"_joaat: { return FARPROC(&shv::exports::scriptUnregister); } break;
		case "scriptUnregisterDepricated"_joaat: { return FARPROC(&shv::exports::scriptUnregisterDepricated); } break;
		case "scriptWait"_joaat: { return FARPROC(&shv::exports::scriptWait); } break;
		case "worldGetAllObjects"_joaat: { return FARPROC(&shv::exports::worldGetAllObjects); } break;
		case "worldGetAllPeds"_joaat: { return FARPROC(&shv::exports::worldGetAllPeds); } break;
		case "worldGetAllPickups"_joaat: { return FARPROC(&shv::exports::worldGetAllPickups); } break;
		case "worldGetAllVehicles"_joaat: { return FARPROC(&shv::exports::worldGetAllVehicles); } break;
		}
	}
	RET_CALL_DECL(getProcAddress, hModule, lpProcName);
}