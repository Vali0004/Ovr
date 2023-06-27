#pragma once
#include "rage/classes.h"

class context : public rage::scrNativeCallContext {
public:
	context() {
		reset();
	}
public:
	void reset() {
		Return = &m_retData[0];
		ArgCount = 0;
		Args = &m_argData[0];
		Reset();
		memset(&m_retData, NULL, sizeof(m_retData));
		memset(&m_argData, NULL, sizeof(m_argData));
	}
	template <typename type>
	void push(type value) {
		static_assert(sizeof(type) <= 8);
		rage::scrValue data;
		*(type*)(&data) = value;
		m_argData[ArgCount++] = data;
	}
	template <>
	void push<Vector3>(Vector3 value) {
		push(value.x);
		push(value.y);
		push(value.z);
	}
	template <typename t>
	t getRetVal() {
		return *reinterpret_cast<t*>((u64)Return);
	}
	template <>
	void getRetVal<void>() {}
private:
	rage::scrValue m_retData[10]{};
	rage::scrValue m_argData[100]{};
};