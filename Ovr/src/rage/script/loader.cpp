#include "loader.h"
#include "util/util.h"

namespace rage::ysc {
	loader::loader() {
		m_hash = "code_controller"_joaat;
		m_thread = util::classes::getGtaThread(m_hash);
		m_program = util::classes::getScrProgram(m_hash);
	}
	loader::~loader() {
		if (m_thread) {
			m_thread->m_serialised.m_state = rage::eThreadState::halted;
		}
		if (m_bytecode) {
			m_bytecode.reset();
		}
	}
	template <typename t>
	void mcpy(t dst, const t src, u64 size) {
		for (u64 i{}; i != size; ++i) {
			dst[i] = src[i];
		}
	}
	void loader::setThread(fnptr<void(program&)> callback) {
		m_bytecode = MakeSmartPointer<program>(callback);
		if (!m_bytecode->m_isGood) {
			LOG(Fatal, "YSA Loading had an error, please refer to the messages above.");
			return;
		}
		u64 scrProgramAddr{ (u64)m_program };
		m_thread->m_serialised.m_state = rage::eThreadState::halted;
		m_thread->m_serialised.m_pointer_count = NULL;
		m_thread->m_serialised.m_catch_pointer_count = NULL;
		m_thread->m_serialised.m_frame_pointer = NULL;
		m_thread->m_serialised.m_catch_frame_pointer = NULL;
		m_thread->m_pointer_count_cleanup = NULL;
		for (uint32_t i{}; i != m_bytecode->m_codePageCollection.codePages.size(); ++i) {
			mcpy(m_program->m_code_blocks[i], m_bytecode->m_codePageCollection.codePages[i], m_bytecode->m_codePageCollection.getCodePageSize(i));
		}
		m_program->m_code_size = m_bytecode->m_codePageCollection.getCodePageSize();
		for (uint32_t i{}; i != m_bytecode->m_stringPageCollection.stringHeaps.size(); ++i) {
			mcpy(const_cast<char*>(m_program->m_string_heaps[i]), m_bytecode->m_stringPageCollection.stringHeaps[i], m_bytecode->m_stringPageCollection.getStringChunkSize(i));
		}
		m_program->m_string_heap_size = m_bytecode->m_stringPageCollection.getStringHeapSize();
		for (uint32_t i{}; i != m_bytecode->m_natives.size(); ++i) {
			m_program->m_natives[i] = g_invoker.getNativeCmd(m_bytecode->m_natives[i]);
		}
		m_program->m_native_count = m_bytecode->m_natives.size();
		m_program->m_global_count = 0x1337;
		if (m_bytecode->m_staticCount) {
			m_program->m_local_count = m_bytecode->m_staticCount;
		}
		if (m_bytecode->m_stackSize) {
			m_thread->m_serialised.m_stack_size = m_bytecode->m_stackSize;
		}
		m_thread->m_serialised.m_state = rage::eThreadState::running;
	}
}