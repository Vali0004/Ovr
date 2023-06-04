#pragma once
#include "pch/pch.h"

class mem {
public:
	mem(void* p = nullptr) : m_ptr(p) {}
	mem(uintptr_t p) : mem((void*)p) {}
public:
	template <typename t>
	std::enable_if_t<std::is_pointer_v<t>, t> as() {
		return static_cast<t>(m_ptr);
	}
	template <typename t>
	std::enable_if_t<std::is_lvalue_reference_v<t>, t> as() {
		return *static_cast<std::add_pointer_t<std::remove_reference_t<t>>>(m_ptr);
	}
	template <typename t>
	std::enable_if_t<std::is_same_v<t, uintptr_t>, t> as() {
		return (t)m_ptr;
	}
	mem add(int v) {
		return mem(as<uintptr_t>() + v);
	}
	mem sub(int v) {
		return mem(as<uintptr_t>() - v);
	}
	mem rip() {
		return add(as<int32_t&>()).add(4);
	}
	mem mov() {
		return add(3).rip();
	}
	mem cmp() {
		return add(3).rip();
	}
	mem lea() {
		return add(2).rip();
	}
	mem call() {
		return add(1).rip();
	}
	operator bool() {
		return m_ptr;
	}
private:
	void* m_ptr{};
};

template <typename t>
inline uintptr_t getMemberTypeCall(t function) {
	return ((u64(*)(t))u64(function))(function);
}
template <typename c, typename m>
inline u64 getMember(m c::* function) {
	union memberCast {
		m c::* function;
		struct {
			void* ptr;
			void* offset;
		};
	};
	memberCast cast{};
	if (sizeof(cast.function) != sizeof(cast.ptr)) {
		return getMemberTypeCall(function);
	}
	cast.function = function;
	return reinterpret_cast<u64>(cast.ptr);
}