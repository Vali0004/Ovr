#include "invoker.h"

void invoker::cache() {
	for (auto& p : g_table) {
		m_cache.insert({ p.o, pointers::g_nativeRegistrationTable->get_handler(p.u) });
	}
}
void invoker::begin() {
	m_context.reset();
}
void invoker::end(rage::scrNativeHash hash) {
	if (rage::scrCmd cmd{ getNativeCmd(hash) }) {
		spoofCall(pointers::g_jmpRbxRegister, cmd, dynamic_cast<rage::scrNativeCallContext*>(&m_context));
		m_context.VectorSpace.CopyReferencedParametersOut();
	}
	else {
		LOG(Fatal, "Failed to get command 0x{:X}", hash);
	}
}