#include "loader.h"
#include "util/util.h"

namespace rage::ysc {
	loader::loader() {
		m_hash = "code_controller"_joaat;
		m_gtaThread = util::classes::getGtaThread(m_hash);
		m_scrProgram = util::classes::getScrProgram(m_hash);
	}
	loader::~loader() {
		if (m_gtaThread) {
			m_gtaThread->m_serialised.m_state = rage::eThreadState::blocked;
		}
		if (m_program) {
			m_program.reset();
		}
	}

	void loader::setThread(fnptr<void(program&)> callback) {
		m_program = MakeSmartPointer<program>(callback);
		m_gtaThread->m_serialised.m_state = rage::eThreadState::blocked;
		m_gtaThread->m_serialised.m_pointer_count = NULL;
		m_gtaThread->m_serialised.m_frame_pointer = NULL;
		m_gtaThread->m_serialised.m_catch_pointer_count = NULL;
		m_gtaThread->m_pointer_count_cleanup = NULL;
		for (u32 i{}; i != m_program->m_codePageCollection.codePages.size(); ++i) {
			memcpy(m_scrProgram->m_code_blocks[i], m_program->m_codePageCollection.codePages[i], m_program->m_codePageCollection.getCodePageSize(i));
		}
		m_scrProgram->m_code_size = m_program->m_codePageCollection.getCodePageSize();
		for (u32 i{}; i != m_program->m_stringPageCollection.stringHeaps.size(); ++i) {
			memcpy((void*)m_scrProgram->m_string_heaps[i], m_program->m_stringPageCollection.stringHeaps[i], m_program->m_stringPageCollection.getStringChunkSize(i));
		}
		m_scrProgram->m_code_size = m_program->m_codePageCollection.getCodePageSize();
		for (uint32_t i{}; i != m_program->m_natives.size(); ++i) {
			m_scrProgram->m_natives[i] = g_invoker.getNativeCmd(m_program->m_natives[i]);
		}
		m_scrProgram->m_native_count = m_program->m_natives.size();
		m_scrProgram->m_global_count = 0x1337;
		m_gtaThread->m_serialised.m_state = rage::eThreadState::running;
	}
}