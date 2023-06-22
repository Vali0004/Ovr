#pragma once
#include "pch/pch.h"
#include "rage/classes.h"

namespace commands {
	union value {
		const char* string;
		bool toggle;
		float floating_point;
		int8_t i8;
		uint8_t u8;
		int16_t i16;
		uint16_t u16;
		int32_t i32;
		uint32_t u32;
		int64_t i64;
		uint64_t u64;
		CNetGamePlayer* game_player;
		rage::netPlayer* net_player;
		rage::rlGamerInfo* gamer_info;
	};
	enum class eValueType : uint8_t {
		String,
		Boolean,
		FloatingPoint,
		Int8,
		UInt8,
		Int16,
		UInt16,
		Int32,
		UInt32,
		Int64,
		UInt64,
		GamePlayer,
		NetPlayer,
		GamerInfo,
		NotNeeded
	};
	class typedValue {
	public:
		typedValue(value value, eValueType type) : m_value(value), m_type(type) {}
		typedValue(value value) : typedValue(value, eValueType::NotNeeded) {}
		typedValue(eValueType type) : typedValue({}, type) {}
		typedValue() {}
		value m_value{};
		eValueType m_type{};
	};
	class hotkey {
	public:
		std::vector<int> m_keys{};
		bool m_enabled{};
		void add_hotkey(int key);
		bool pressed();
	};
	enum class eCommandType : uint8_t {
		AbstractCommand,
		ToggleCommand,
		IntCommand,
		FloatCommand,
		ToggleIntCommand,
		ToggleFloatCommand,
		ActionCommand,
		ProtectionCommand,
		VariadicCommand
	};
	class abstractCommand {
	public:
		abstractCommand(std::string id, std::string name, std::string description, hotkey hotkey, eCommandType type, bool looped) :
			m_id(id), m_name(name), m_description(description), m_lookupId(rage::joaat(m_id)), m_hotkey(hotkey), m_type(type), m_looped(looped)
		{}
		abstractCommand(std::string id, std::string name, std::string description, eCommandType type, bool looped) : abstractCommand(id, name, description, {}, type, looped) {}
		abstractCommand(std::string id, std::string name, eCommandType type, bool looped) : abstractCommand(id, name, {}, type, looped) {}
		abstractCommand(std::string id, eCommandType type, bool looped) : abstractCommand(id, {}, type, looped) {}
		virtual ~abstractCommand() {
			deallocate();
		}
		virtual void init() {
			if (!m_looped) {
				if (m_type != eCommandType::AbstractCommand && m_type != eCommandType::IntCommand && m_type != eCommandType::FloatCommand && m_type != eCommandType::ActionCommand)
					m_looped = true;
			}
			m_intialized = true;
		}
		virtual void run() {}
		void deallocate() {
			m_values.clear();
			m_hotkey.m_keys.clear();
		}
		void push_value(typedValue& v) {
			m_values.emplace_back(&v);
		}
		typedValue* get_value(size_t index) {
			if (index > m_values.size() - 1) {
				throw std::runtime_error("Value index cannot be over value size!");
			}
			return m_values[index];
		}
		value& get(size_t index) {
			return get_value(index)->m_value;
		}
		value& v() {
			return get(0);
		}
		void add_hotkey(int key) {
			m_hotkey.add_hotkey(key);
		}
		bool has_value() {
			return !m_values.empty() && value_count();
		}
		size_t value_count() {
			return m_values.size();
		}
	public:
		eCommandType m_type{};
		std::string m_id{};
		std::string m_name{};
		std::string m_description{};
		uint32_t m_lookupId{};
		hotkey m_hotkey{};
		bool m_looped{};
		std::vector<std::string> m_buffer{};
		std::string m_context{};
	private:
		bool m_intialized{};
		std::vector<typedValue*> m_values{};
	};
}