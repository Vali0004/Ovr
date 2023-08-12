#pragma once
#include "util/util.h"
#include "memory/patch.h"
#include "memory/scanner.h"

class GameVMData {
public:
	GameVMData(rage::scrProgram* program) : m_size(program->m_code_size), m_pageCount(program->get_num_code_pages()) {
		u8** pages{ new u8 * [m_pageCount] };
		for (u32 i{}; i != m_pageCount; ++i) {
			u32 codePageSize{ program->get_code_page_size(i) };
			pages[i] = new u8[codePageSize];
			memcpy(pages[i], program->m_code_blocks[i], codePageSize);
		}
	}
	~GameVMData() {
		for (u32 i{}; i != m_pageCount; ++i) {
			delete[] m_code[i];
		}
		delete[] m_code;
		m_code = nullptr;
	}
public:
	void set(i32 ip, std::vector<u8>& data) {
		memcpy(getAddress(ip), data.data(), data.size());
	}
	u8* getAddress(u32 index) {
		return &m_code[index >> 14][index & 0x3FFF];
	}
	u8** m_code{};
	u32 m_size{};
private:
	u32 m_pageCount{};
};
class GameVMPatch {
public:
	GameVMPatch(const std::string& pattern, i32 offset, const std::vector<u8>& patch) :
		m_patternBytes(createBytesFromString(pattern)), m_offset(offset), m_patch(patch) {
	}
public:
	void getCodeLocation(GameVMData* data) {
		for (u32 i{}; i != data->m_size; ++i) {
			if (doesMemoryMatch(data->getAddress(i), m_patternBytes.data(), m_patternBytes.size())) {
				m_ip = i + m_offset;
				break;
			}
		}
	}
	void createOriginalBytes(GameVMData* data) {
		getCodeLocation(data);
		for (i32 i{}; i != m_patch.size(); ++i) {
			m_original.push_back(*data->getAddress(m_ip + i));
		}
	}
	void set(GameVMData* data, bool restore) {
		data->set(m_ip, restore ? m_original : m_patch);
	}
private:
	std::vector<std::optional<u8>> m_patternBytes{};
	i32 m_offset{};
	std::vector<u8> m_patch{};
	std::vector<u8> m_original{};
	i32 m_ip{};
};
class GameVMPatchManager {
public:
	GameVMPatchManager(u8** code, rage::scrProgram* program) :
		m_code(code), m_program(program), m_data(new GameVMData(m_program)) {

	}
	~GameVMPatchManager() {

	}
public:
	void createPatch(u32 id, const std::string& pattern, i32 offset, const std::vector<u8>& patch) {
		m_patches.insert({ id, new GameVMPatch(pattern, offset, patch) });
		m_patches.at(id)->createOriginalBytes(m_data);
	}
	void set(u32 id, bool restore = false) {
		m_patches.at(id)->set(m_data, restore);
	}
	void removePatch(u32 id) {
		set(id, true);
		delete m_patches.at(id);
		m_patches.erase(id);
	}
	void cleanup() {
		for (auto& entry : m_patches) {
			delete entry.second;
		}
	}
private:
	u8** m_code{};
	rage::scrProgram* m_program{};
	GameVMData* m_data{};
	std::unordered_map<u32, GameVMPatch*> m_patches{};
};
class GameVMGuard {
public:
	GameVMGuard(rage::scrProgram* program, rage::scrThread::Serialised* thread, u8** opcodeTable) :
		m_program(program), m_thread(thread), m_unmodifiedOpcodes(opcodeTable) {
		//m_patchManager = std::make_unique<GameVMPatchManager>(opcodeTable, program);
	}
	~GameVMGuard() {
		//m_patchManager.reset();
	}
public:
	void addPatch(u32 id, std::string pattern, i32 offset, std::vector<u8> patch, bool apply = true) {
		std::unique_lock lock(m_mutex);
		m_patchManager->createPatch(id, pattern, offset, patch);
		if (apply) {
			applyPatch(id);
		}
	}
	void applyPatch(u32 id) {
		std::unique_lock lock(m_mutex);
		m_patchManager->set(id);
	}
	void restorePatch(u32 id) {
		std::unique_lock lock(m_mutex);
		m_patchManager->set(id, true);
	}
public:
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
			if (fn) {
				fn();
			}
		}
	}
private:
	std::recursive_mutex m_mutex{};
	std::unique_ptr<GameVMPatchManager> m_patchManager{};
	rage::scrThread::Serialised* m_thread{};
	rage::scrProgram* m_program{};
	u8** m_unmodifiedOpcodes{};
	std::stack<std::function<void()>> m_callbacks{};
};
class GlobalGameVMGuard {
public:
	GlobalGameVMGuard() {}
public:
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
	void WaitUntilGuardIsCreated(u32 hash, std::function<void(GameVMGuard*)> callback) {
		util::async([&] {
			while (!GetGuard(hash)) {
				std::this_thread::sleep_for(100ms);
			}
			callback(GetGuard(hash));
		});
	}
private:
	std::map<u32, GameVMGuard*> m_guards{};
};
inline GlobalGameVMGuard g_GlobalGameVMGuard{};