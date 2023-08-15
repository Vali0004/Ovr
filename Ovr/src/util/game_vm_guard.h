#pragma once
#include "util/util.h"
#include "memory/patch.h"
#include "memory/scanner.h"

template <typename f, typename ...a>
void runUnderThread(u32 hash, f&& callback, a &&...args) {
	rage::tlsContext* threadLocalStorage{ rage::tlsContext::get() };
	for (GtaThread* thread : *pointers::g_gtaThreads) {
		if (!thread || !thread->m_serialised.m_thread_id || thread->m_serialised.m_script_hash != hash) {
			continue;
		}
		rage::scrThread* oThread{ threadLocalStorage->m_script_thread };
		threadLocalStorage->m_script_thread = thread;
		threadLocalStorage->m_is_script_thread_active = true;
		std::invoke(std::forward<f>(callback), std::forward<a>(args)...);
		threadLocalStorage->m_script_thread = oThread;
		threadLocalStorage->m_is_script_thread_active = oThread != nullptr;
		return;
	}
}
class GameVMGuard {
public:
	GameVMGuard(rage::scrProgram* program, rage::scrThread::Serialised* thread, u8** opcodeTable) :
		m_program(program), m_thread(thread), m_unmodifiedOpcodes(opcodeTable) {}
	~GameVMGuard() {}

	void setCallbackOnThread(std::function<void()> callback) {
		std::unique_lock lock(m_mutex);
		m_callbacks.push(callback);
	}
	void runCallbacks() {
		std::unique_lock lock(m_mutex);
		if (!m_callbacks.empty()) {
			auto fn{ std::move(m_callbacks.top()) };
			m_callbacks.pop();
			lock.unlock();
			runUnderThread(m_thread->m_script_hash, fn);
		}
	}
private:
	std::recursive_mutex m_mutex{};
	rage::scrThread::Serialised* m_thread{};
	rage::scrProgram* m_program{};
	u8** m_unmodifiedOpcodes{};
	std::stack<std::function<void()>> m_callbacks{};
};
class GlobalGameVMGuard {
public:
	GlobalGameVMGuard() {}

	GameVMGuard* CreateGuardForThread(rage::scrProgram* program, rage::scrThread::Serialised* thread, u8** opcodeTable) {
		GameVMGuard* guard{ new GameVMGuard(program, thread, opcodeTable) };
		m_guards.insert({ program->m_name_hash, std::move(guard) });
		return m_guards[program->m_name_hash];
	}
	GameVMGuard* GetGuard(u32 hash) {
		if (auto it{ m_guards.find(hash) }; it != m_guards.end()) {
			return it->second;
		}
		return nullptr;
	}
	void Cleanup() {
		for (auto& entry : m_guards) {
			delete entry.second;
		}
	}
private:
	std::map<u32, GameVMGuard*> m_guards{};
};
inline GlobalGameVMGuard g_GlobalGameVMGuard{};