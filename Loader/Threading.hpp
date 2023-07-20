#pragma once
#include "Pch.hpp"

enum class ThreadState { None, Idle, Started, Finished };
class ThreadHandler {
public:
	ThreadHandler() = default;
	ThreadHandler(std::string id, std::function<void()> func) : ThrId(id), Thr([this, func] { func(); }) {
	}
	std::string Id() {
		return ThrId;
	}
	void Detach() {
		Thr.detach();
	}
private:
	std::string ThrId;
	std::thread Thr;
};
class TimeHelper {
public:
	TimeHelper(std::string threadId, size_t id, ThreadState state, std::chrono::high_resolution_clock::time_point sleepTime) : ThreadId(threadId), TimerId("TID_") {
		SetTId(id);
		SetState(state);
		SleepTime = sleepTime;
	}
public:
	void SetState(ThreadState state) {
		ThreadState = state;
	}
	void SetTId(size_t id) {
		TimerId += std::to_string(id);
	}
public:
	ThreadState State() {
		return ThreadState;
	}
	std::optional<std::chrono::high_resolution_clock::time_point> Time() {
		return SleepTime;
	}
	std::string Id() {
		return ThreadId;
	}
	std::string TId() {
		return TimerId;
	}
private:
	std::string ThreadId{};
	std::string TimerId{};
	ThreadState ThreadState{};
	std::chrono::high_resolution_clock::time_point SleepTime{};
};
class Threading {
public:
	void CreateLoopedThread(std::function<void()> func, bool* thrKill, std::string thrId);
	void Wait(std::optional<std::chrono::high_resolution_clock::duration> forTime = std::nullopt, bool arrayPush = true, std::string thrId = "SecondaryThread");
	ThreadState IsThreadReady(std::string thrId);
	void ThreadTick(std::function<void()> func, bool* kill, std::string id);
	void ThreadLoop();
	void Queue(std::function<void()> func, std::string thrId = "SecondaryThread");
	void FunctionInvoke(std::function<void()> func, std::string invokePlace);
	void Clear();
private:
	std::vector<std::unique_ptr<ThreadHandler>> m_ThreadPool{};
	std::deque<std::pair<std::function<void()>, std::string>> m_FunctionPool{};
	std::vector<TimeHelper> m_TimePool{};
	std::recursive_mutex m_Mutex{};
	bool MainThreadKilled{};
	bool SecondaryThreadKilled{};
};
inline Threading g_Threading{};