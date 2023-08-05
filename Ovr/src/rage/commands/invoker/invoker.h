#pragma once
#include "pch/pch.h"
#include "core/logger.h"
#include "rage/commands/dump_commands.h"
#include "memory/pointers.h"
#include "rage/classes.h"
#include "call_context.h"

class invoker {
public:
	void cache();
	void begin();
	void end(rage::scrNativeHash hash);
public:
	template <typename t>
	void push(t&& value) {
		m_context.push(std::forward<t>(value));
	}
	template <typename t>
	t getRet() {
		return m_context.getRetVal<t>();
	}
	template <typename t>
	t* getRetPointer() {
		return reinterpret_cast<t*>(m_context.ResultPtr);
	}
	rage::Cmd getNativeCmd(u64 hash) {
		if (auto p{ m_cache.find(hash) }; p != m_cache.end()) {
			return p->second;
		}
		return pointers::g_nativeRegistrationTable->get_handler(correctNative(hash));
	}
	u64 getNativeHash(rage::Cmd cmd) {
		for (auto& pair : m_cache) {
			if (pair.second == cmd) {
				return pair.first;
			}
		}
		return 0;
	}
	u64 correctNative(u64 hash) {
		if (auto lookup{ util::game::commands::getLookupFromHash(hash) }; lookup.newHash) {
			return lookup.newHash;
		}
		return 0;
	}
private:
	std::map<rage::scrNativeHash, rage::Cmd> m_cache{};
	context m_context{};
};
inline invoker g_invoker{};