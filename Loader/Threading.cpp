#include "Threading.hpp"

void Threading::CreateLoopedThread(std::function<void()> func, bool* thrKill, std::string thrId) {
	m_ThreadPool.push_back(std::make_unique<ThreadHandler>(thrId, [=] { ThreadTick(func, thrKill, thrId); }));
}
void Threading::Wait(std::optional<std::chrono::high_resolution_clock::duration> forTime, bool arrayPush , std::string thrId) {
	if (arrayPush)
		m_TimePool.push_back({ thrId, (size_t)rand(), ThreadState::Started, std::chrono::steady_clock::now() + *forTime });
	std::this_thread::sleep_until(std::chrono::steady_clock::now() + *forTime);
}
ThreadState Threading::IsThreadReady(std::string thrId) {
	for (int i{}; i != m_TimePool.size(); ++i) {
		if (std::chrono::high_resolution_clock::now() >= *m_TimePool[i].Time()) {
			m_TimePool.erase(m_TimePool.begin() + 0);
			m_TimePool[i].SetState(ThreadState::Finished);
		}
		else if (!m_TimePool[i].Time()) {
			m_TimePool[i].SetState(ThreadState::Idle);
		}
		return m_TimePool[i].State();
	}
	return ThreadState::None;
}
void Threading::ThreadTick(std::function<void()> func, bool* kill, std::string id) {
	auto ThreadKill = *kill;
	while (!ThreadKill && g_Running) {
		if (GetAsyncKeyState(VK_END))
			g_Running = false;
		if (IsThreadReady(id) != ThreadState::Started) {
			func();
		}
		std::this_thread::sleep_for(1ms);
	}
}
void Threading::ThreadLoop() {
	CreateLoopedThread([=] {
		if (g_Running) {
			decltype(m_FunctionPool)::value_type task; {
				std::lock_guard lock(m_Mutex);
				if (!m_FunctionPool.empty()) {
					task = std::move(m_FunctionPool.front());
					m_FunctionPool.pop_front();
				}
			}
			if (task.first)
				FunctionInvoke(task.first, task.second);
		}
	}, &SecondaryThreadKilled, "SecondaryThread");
}
void Threading::Queue(std::function<void()> func, std::string thrId) {
	std::lock_guard lock(m_Mutex);
	m_FunctionPool.push_back({ func, thrId });
}
void Threading::FunctionInvoke(std::function<void()> func, std::string invokePlace) {
	std::invoke(func);
}
void Threading::Clear() {
	for (auto& Thread : m_ThreadPool) {
		Thread->Detach();
	}
	m_ThreadPool.clear();
}