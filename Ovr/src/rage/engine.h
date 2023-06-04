#pragma once
#include "classes.h"
#include "joaat.h"
#include "memory/pointers.h"

namespace engine {
	class thread : public GtaThread {
	public:
		void init() {
			return pointers::g_scrThreadInit(this);
		}
		virtual void do_run() = 0;
		virtual rage::eThreadState reset(uint32_t script_hash, void* args, uint32_t arg_count) {
			RtlZeroMemory(&m_context, sizeof(m_context));
			m_context.m_state = rage::eThreadState::sleeping;
			m_context.m_script_hash = script_hash;
			m_context.m_min_pc = -1;
			m_context.m_max_pc = -1;
			m_context.m_priority = 1;
			init();
			m_exit_message = "Normal exit";
			return m_context.m_state;
		}
		virtual rage::eThreadState run() {
			auto tls{ rage::tlsContext::get() };
			if (!m_handler) {
				pointers::g_scriptHandlerMgr->attach_thread(this);
				m_safe_for_network_game = true;
			}
			auto ogThr = tls->m_script_thread;
			tls->m_script_thread = this;
			if (m_context.m_state != rage::eThreadState::killed) {
				do_run();
			}
			tls->m_script_thread = ogThr;
			return m_context.m_state;
		}
		virtual rage::eThreadState tick(uint32_t ops_to_execute) {
			return pointers::g_scrThreadTick(this, ops_to_execute);
		}
		virtual void kill() {
			return pointers::g_scrThreadKill(this);
		}
	};
	template <typename t>
	using pgCollection = rage::atArray<t>;
	template <typename t>
	using pgPtrCollection = rage::atArray<t*>;
	inline std::vector<std::pair<thread*, thread*>> g_ownedThreads;
	inline pgPtrCollection<thread>* getThreadCollection() {
		return (pgPtrCollection<thread>*)pointers::g_gtaThreads;
	}
	inline uint32_t& getThreadID() {
		return *pointers::g_threadId;
	}
	inline uint32_t& getThreadCount() {
		return *pointers::g_threadCount;
	}
	inline void createThread(thread* thread) {
		auto& collection{ *getThreadCollection() };
		auto& id{ getThreadID() };
		auto& count{ getThreadCount() };
		int slot{};
		//Get slot
		for (auto& thr : collection) {
			auto& ctx{ thr->m_context };
			if (!ctx.m_thread_id)
				break;
			slot++;
		}
		bool didWeGetSlot{ slot != collection.size() };
		if (didWeGetSlot) {
			std::string name{ std::format("thr_{}", count + 1) };
			auto& ctx{ thread->m_context };
			strcpy_s(thread->m_name, name.c_str());
			thread->m_script_hash = rage::joaat(thread->m_name);
			thread->reset(thread->m_script_hash, nullptr, 0);
			thread->m_can_be_paused = false;
			if (!id) {
				id++;
			}
			ctx.m_thread_id = id;
			count++;
			id++;
			auto og{ collection[slot] };
			collection[slot] = thread;
			g_ownedThreads.push_back(std::make_pair(thread, og));
			LOG(FOREGROUND_INTENSITY, "Info", "Successfully created thread '{}' at id {}", thread->m_name, ctx.m_thread_id);
			LOG(FOREGROUND_INTENSITY, "Info", "Yoinked {}'s thread slot ({})", og->m_name, ctx.m_thread_id);
		}
	}
	inline void cleanupThreads() {
		std::reverse(g_ownedThreads.begin(), g_ownedThreads.end());
		auto& collection{ *getThreadCollection() };
		int slot{};
		for (auto& thr : collection) {
			for (auto& [ownedThr, ogThr] : g_ownedThreads) {
				if (thr == ownedThr) {
					collection[slot] = ogThr;
				}
			}
			slot++;
		}
		g_ownedThreads.clear();
	}
}