#pragma once
#include "collections.h"
#include "program.h"
#include "rage/commands/invoker/invoker.h"

namespace rage::ysc {
	class loader {
	public:
		loader();
		~loader();

		void setThread(fnptr<void(program&)> callback);
	private:
		u32 m_hash{};
		GtaThread* m_thread{};
		rage::scrProgram* m_program{};
	public:
		SmartPointer<program> m_bytecode{};
	};
	inline SmartPointer<loader> g_loader{};
}