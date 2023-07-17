#pragma once
#include "pch/pch.h"
#include "rage/classes.h"
#include "core/logger.h"

namespace commands {
	union value {
		const char* string;
		bool toggle;
		float floating_point;
		i8 i8;
		u8 u8;
		i16 i16;
		u16 u16;
		i32 i32;
		u32 u32;
		i64 i64;
		u64 u64;
		CNetGamePlayer* game_player;
		rage::netPlayer* net_player;
		rage::rlGamerInfo* gamer_info;
	};
	enum class eValueType : u8 {
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
		value& operator*() {
			return m_value;
		}
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
	enum class eCommandType : u8 {
		AbstractCommand,
		ToggleCommand,
		IntCommand,
		FloatCommand,
		ToggleIntCommand,
		ToggleFloatCommand,
		ActionCommand,
		ProtectionCommand,
		SectionProtectionCommand,
		StringCommand,
		HashCommand,
		VariadicCommand
	};
	class abstractCommand {
	public:
		abstractCommand(std::string id, std::string name, std::string description, hotkey hotkey, eCommandType type, bool looped) :
			m_id(id), m_name(name), m_description(description), m_hotkey(hotkey), m_type(type), m_looped(looped)
		{}
		abstractCommand(std::string id, std::string name, std::string description, eCommandType type, bool looped) : abstractCommand(id, name, description, {}, type, looped) {}
		abstractCommand(std::string id, std::string name, eCommandType type, bool looped) : abstractCommand(id, name, {}, type, looped) {}
		abstractCommand(std::string id, eCommandType type, bool looped) : abstractCommand(id, {}, type, looped) {}
		virtual ~abstractCommand() {
			deallocate();
		}
		virtual void init() {
			m_intialized = true;
		}
		virtual void run() {}
		void deallocate() {
			m_values.clear();
			m_hotkey.m_keys.clear();
		}
		ccp id() {
			return m_id.c_str();
		}
		void push_value(typedValue& v) {
			m_values.emplace_back(&v);
		}
		typedValue* get_value(size_t index) {
			if (index > m_values.size() - 1) {
				LOG_DEBUG("Value index cannot be over value size!");
				return nullptr;
			}
			return m_values[index];
		}
		value& get(size_t index) {
			typedValue* g{ get_value(index) };
			if (g)
				return g->m_value;
			static value dummy{};
			return dummy;
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
		hotkey m_hotkey{};
		bool m_looped{};
		std::vector<std::string> m_buffer{};
		std::string m_context{};
	private:
		bool m_intialized{};
		std::vector<typedValue*> m_values{};
	};
}