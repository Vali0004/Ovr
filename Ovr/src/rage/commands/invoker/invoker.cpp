#include "invoker.h"
#include "hooking/hooking.h"

void invoker::cache() {
	for (auto& p : util::game::commands::g_map) {
		m_cache.insert({ p.oldHash, pointers::g_nativeRegistrationTable->get_handler(p.newHash) });
	}
	LOG(Info, "Cached {} native command handlers from the game's registry", m_cache.size());
}
void invoker::begin() {
	m_context.reset();
}
void invoker::end(rage::scrNativeHash hash) {
	if (rage::Cmd cmd{ getNativeCmd(hash) }) {
		g_statistics.m_nativesInvokedByUs++;
		cmd(&m_context);
		m_context.CopyReferencedParametersOut();
	}
	else {
		LOG(Fatal, "Failed to get command 0x{:X}", hash);
	}
}