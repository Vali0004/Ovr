#include "manager.h"
#include "memory/pointers.h"

namespace commands {
	void hotkey::add_hotkey(i32 key) {
		m_keys.emplace_back(key);
	}
	bool hotkey::pressed() {
		if (m_enabled)
			return false;
		bool pressed{};
		for (auto& k : m_keys) {
			if (GetForegroundWindow() == pointers::g_hwnd) {
				if (GetAsyncKeyState(k) & 0x1) {
					pressed = true;
				}
			}
		}
		return pressed;
	}
	void manager::remove(ccp id) {
		if (auto it{ m_commands.find(id) }; it != m_commands.end()) {
			delete it->second;
			m_commands.erase(it);
		}
	}
	void manager::init() {
		for (auto& e : m_commands) {
			e.second->init();
		}
		/*if (util::files::input("Config.json").is_open()) {
			fromFile("Config");
		}
		else {
			toFile("Config");
		}*/
	}
	void manager::tick() {
		for (auto& e : m_commands) {
			auto& c{ e.second };
			if (c->m_type == eCommandType::ProtectionCommand || c->m_type == eCommandType::SectionProtectionCommand)
				continue;
			if (c->m_looped) {
				c->run();
			}
			else if (c->m_hotkey.pressed()) {
				ONCE_PER_FRAME({ c->run(); });
			}
		}
	}
	void manager::clear() {
		for (auto& e : m_commands) {
			auto& c{ e.second };
			delete c;
		}
	}

	//void manager::fromFile(const std::string& name) {
	//	if (name.empty()) {
	//		return;
	//	}
	//	std::ifstream file{ util::files::input(name + ".json")};
	//	if (file) {
	//		nlohmann::json json{};
	//		file >> json;
	//		for (const auto& entry : getCommands()) {
	//			const auto& command{ entry.second };
	//			if (json["features"].contains(command->id())) {
	//				command->m_json = json["features"][command->id()];
	//				//command->deserialise();
	//			}
	//		}
	//	}
	//	else {
	//		LOG(Commands, "{} is not a valid config.", name);
	//	}
	//}
	//void manager::toFile(const std::string& name) {
	//	if (name.empty()) {
	//		return;
	//	}
	//	nlohmann::json json{};
	//	for (const auto& entry : getCommands()) {
	//		const auto& command{ entry.second };
	//		command->serialise();
	//		json["features"][command->id()].push_back(command->m_json);
	//	}
	//	std::ofstream file{ util::files::output(name + ".json")};
	//	if (file) {
	//		file << json.dump(1, '	') << std::endl;
	//	}
	//}
}