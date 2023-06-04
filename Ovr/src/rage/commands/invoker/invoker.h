#pragma once
#include "pch/pch.h"
#include "rage/commands/conversion_table.h"
#include "memory/pointers.h"
#include "rage/classes.h"
#include "spoof/call_spoof.h"
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
		return reinterpret_cast<t*>(m_context.Return);
	}
private:
	std::map<rage::scrNativeHash, rage::scrCmd> m_cache{};
	context m_context{};
};
inline invoker g_invoker{};