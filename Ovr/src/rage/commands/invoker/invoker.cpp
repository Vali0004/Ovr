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
	if (auto p{ m_cache.find(hash) }; p != m_cache.end()) {
		rage::scrCmd cmd{ p->second };
		if (!cmd) {
			LOG(Fatal, "Failed to get 0x{:X}'s command from entries", hash);
			return;
		}
		spoofCall(pointers::g_jmpRbxRegister, cmd, dynamic_cast<rage::scrNativeCallContext*>(&m_context));
		m_context.VectorSpace.CopyReferencedParametersOut();
	}
	else {
		LOG(Fatal, "Failed to get 0x{:X}'s command from cache", hash);
	}
}