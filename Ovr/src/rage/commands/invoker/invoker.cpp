#include "invoker.h"
#include "hooking/hooking.h"

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
		g_statistics.m_nativesInvokedByUs++;
		cmd(&m_context);
		m_context.VectorSpace.CopyReferencedParametersOut();
	}
	else {
		LOG(Fatal, "Failed to get command 0x{:X}", hash);
	}
}