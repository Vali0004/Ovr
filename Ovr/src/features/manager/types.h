#pragma once
#include "abstract.h"

enum class eProtectionState : u8 {
	Disabled,
	Notify,
	Block,
	BlockAndNotify
};
namespace features {
	class toggleFeature : public abstractFeature {
	public:
		toggleFeature(std::string id, std::string name, std::string description, hotkey hotkey, fnptr<void(toggleFeature*)> callback) :
			abstractFeature(id, name, description, hotkey, eFeatureType::ToggleFeature, true),
			m_callback(callback) {
		}
		toggleFeature(std::string id, std::string name, std::string description, fnptr<void(toggleFeature*)> callback) :
			toggleFeature(id, name, description, {}, callback) {
		}
		toggleFeature(std::string id, std::string name, fnptr<void(toggleFeature*)> callback) :
			toggleFeature(id, name, {}, callback) {
		}
		toggleFeature(std::string id, fnptr<void(toggleFeature*)> callback) :
			toggleFeature(id, {}, callback) {
		}
		~toggleFeature() {
			abstractFeature::~abstractFeature();
		}
		void init() override {
			push_value(m_value);
			abstractFeature::init();
		}
		void run() override {
			if (m_hotkey.pressed()) {
				m_value.m_value.toggle ^= true;
			}
			m_callback(this);
			abstractFeature::run();
		}
	public:
		typedValue m_value{};
	private:
		fnptr<void(toggleFeature*)> m_callback{};
	};
	class intFeature : public abstractFeature {
	public:
		intFeature(std::string id, std::string name, std::string description, hotkey hotkey, fnptr<void(intFeature*)> callback, bool looped = false) :
			abstractFeature(id, name, description, hotkey, eFeatureType::IntFeature, looped),
			m_callback(callback) {
		}
		intFeature(std::string id, std::string name, std::string description, fnptr<void(intFeature*)> callback, bool looped = false) :
			intFeature(id, name, description, {}, callback, looped) {
		}
		intFeature(std::string id, std::string name, fnptr<void(intFeature*)> callback, bool looped = false) :
			intFeature(id, name, {}, callback, looped) {
		}
		intFeature(std::string id, fnptr<void(intFeature*)> callback, bool looped = false) :
			intFeature(id, {}, callback, looped) {
		}
		~intFeature() {
			abstractFeature::~abstractFeature();
		}
	public:
		void init() override {
			push_value(m_value);
			abstractFeature::init();
		}
		void run() override {
			m_callback(this);
			abstractFeature::run();
		}
	public:
		typedValue m_value{};
	private:
		fnptr<void(intFeature*)> m_callback{};
	};
	class toggleIntFeature : public abstractFeature {
	public:
		toggleIntFeature(std::string id, std::string name, std::string description, hotkey hotkey, fnptr<void(toggleIntFeature*)> callback) :
			abstractFeature(id, name, description, hotkey, eFeatureType::ToggleIntFeature, true),
			m_callback(callback) {
		}
		toggleIntFeature(std::string id, std::string name, std::string description, fnptr<void(toggleIntFeature*)> callback) :
			toggleIntFeature(id, name, description, {}, callback) {
		}
		toggleIntFeature(std::string id, std::string name, fnptr<void(toggleIntFeature*)> callback) :
			toggleIntFeature(id, name, {}, callback) {
		}
		toggleIntFeature(std::string id, fnptr<void(toggleIntFeature*)> callback) :
			toggleIntFeature(id, {}, callback) {
		}
		~toggleIntFeature() {
			abstractFeature::~abstractFeature();
		}
	public:
		void init() override {
			push_value(m_toggleValue);
			push_value(m_numberValue);
			abstractFeature::init();
		}
		void run() override {
			if (m_hotkey.pressed()) {
				m_toggleValue.m_value.toggle ^= true;
			}
			m_callback(this);
			abstractFeature::run();
		}
	public:
		typedValue m_toggleValue{};
		typedValue m_numberValue{};
	private:
		fnptr<void(toggleIntFeature*)> m_callback{};
	};
	class floatFeature : public abstractFeature {
	public:
		floatFeature(std::string id, std::string name, std::string description, hotkey hotkey, fnptr<void(floatFeature*)> callback, bool looped = false) :
			abstractFeature(id, name, description, hotkey, eFeatureType::FloatFeature, looped),
			m_callback(callback) {
		}
		floatFeature(std::string id, std::string name, std::string description, fnptr<void(floatFeature*)> callback, bool looped = false) :
			floatFeature(id, name, description, {}, callback, looped) {
		}
		floatFeature(std::string id, std::string name, fnptr<void(floatFeature*)> callback, bool looped = false) :
			floatFeature(id, name, {}, callback, looped) {
		}
		floatFeature(std::string id, fnptr<void(floatFeature*)> callback, bool looped = false) :
			floatFeature(id, {}, callback, looped) {
		}
		~floatFeature() {
			abstractFeature::~abstractFeature();
		}
	public:
		void init() override {
			push_value(m_value);
			abstractFeature::init();
		}
		void run() override {
			m_callback(this);
			abstractFeature::run();
		}
	public:
		typedValue m_value{};
	private:
		fnptr<void(floatFeature*)> m_callback{};
	};
	class toggleFloatFeature : public abstractFeature {
	public:
		toggleFloatFeature(std::string id, std::string name, std::string description, hotkey hotkey, fnptr<void(toggleFloatFeature*)> callback) :
			abstractFeature(id, name, description, hotkey, eFeatureType::ToggleFloatFeature, true),
			m_callback(callback) {
		}
		toggleFloatFeature(std::string id, std::string name, std::string description, fnptr<void(toggleFloatFeature*)> callback) :
			toggleFloatFeature(id, name, description, {}, callback) {
		}
		toggleFloatFeature(std::string id, std::string name, fnptr<void(toggleFloatFeature*)> callback) :
			toggleFloatFeature(id, name, {}, callback) {
		}
		toggleFloatFeature(std::string id, fnptr<void(toggleFloatFeature*)> callback) :
			toggleFloatFeature(id, {}, callback) {
		}
		~toggleFloatFeature() {
			abstractFeature::~abstractFeature();
		}
	public:
		void init() override {
			push_value(m_toggleValue);
			push_value(m_numberValue);
			abstractFeature::init();
		}
		void run() override {
			if (m_hotkey.pressed()) {
				m_toggleValue.m_value.toggle ^= true;
			}
			m_callback(this);
			abstractFeature::run();
		}
	public:
		typedValue m_toggleValue{};
		typedValue m_numberValue{};
	private:
		fnptr<void(toggleFloatFeature*)> m_callback{};
	};
	class actionFeature : public abstractFeature {
	public:
		actionFeature(std::string id, std::string name, std::string description, hotkey hotkey, fnptr<void(actionFeature*)> callback) :
			abstractFeature(id, name, description, hotkey, eFeatureType::ActionFeature, false),
			m_callback(callback) {
		}
		actionFeature(std::string id, std::string name, std::string description, fnptr<void(actionFeature*)> callback) :
			actionFeature(id, name, description, {}, callback) {
		}
		actionFeature(std::string id, std::string name, fnptr<void(actionFeature*)> callback) :
			actionFeature(id, name, {}, callback) {
		}
		actionFeature(std::string id, fnptr<void(actionFeature*)> callback) :
			actionFeature(id, {}, callback) {
		}
		~actionFeature() {
			abstractFeature::~abstractFeature();
		}
	public:
		void init() override {
			abstractFeature::init();
		}
		void run() override {
			m_callback(this);
			abstractFeature::run();
		}
	private:
		fnptr<void(actionFeature*)> m_callback{};
	};
	class protectionFeature : public abstractFeature {
	public:
		protectionFeature(std::string id, std::string name, std::string description) :
			abstractFeature(id, name, description, {}, eFeatureType::ProtectionFeature, false) {
		}
		protectionFeature(std::string id, std::string name) :
			protectionFeature(id, name, {}) {
		}
		protectionFeature(std::string id) :
			protectionFeature(id, {}) {
		}
		~protectionFeature() {
			abstractFeature::~abstractFeature();
		}
		void init() override {
			push_value(m_value);
			abstractFeature::init();
		}
		void run() override {
			abstractFeature::run();
		}
		eProtectionState state() {
			switch (rage::joaat(m_value.m_value.string)) {
			case "Disabled"_joaat: {
				return eProtectionState::Disabled;
			} break;
			case "Disable"_joaat: {
				return eProtectionState::Disabled;
			} break;
			case "Disabl"_joaat: {
				return eProtectionState::Disabled;
			} break;
			case "Disab"_joaat: {
				return eProtectionState::Disabled;
			} break;
			case "Disa"_joaat: {
				return eProtectionState::Disabled;
			} break;
			case "Dis"_joaat: {
				return eProtectionState::Disabled;
			} break;
			case "Di"_joaat: {
				return eProtectionState::Disabled;
			} break;
			case "D"_joaat: {
				return eProtectionState::Disabled;
			} break;
			case "Notify"_joaat: {
				return eProtectionState::Notify;
			} break;
			case "Notif"_joaat: {
				return eProtectionState::Notify;
			} break;
			case "Noti"_joaat: {
				return eProtectionState::Notify;
			} break;
			case "Not"_joaat: {
				return eProtectionState::Notify;
			} break;
			case "No"_joaat: {
				return eProtectionState::Notify;
			} break;
			case "N"_joaat: {
				return eProtectionState::Notify;
			} break;
			case "Block"_joaat: {
				return eProtectionState::Block;
			} break;
			case "Bloc"_joaat: {
				return eProtectionState::Block;
			} break;
			case "Blo"_joaat: {
				return eProtectionState::Block;
			} break;
			case "Bl"_joaat: {
				return eProtectionState::Block;
			} break;
			case "B"_joaat: {
				return eProtectionState::Block;
			} break;
			case "BlockAndNotify"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BlockAndNotif"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BlockAndNoti"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BlockAndNot"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BlockAndNo"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BlockAndN"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BlockAnd"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BlockAn"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BlockA"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BAndNotify"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BAndNotif"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BAndNoti"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BAndNot"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BAndNo"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BAndN"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			case "BAN"_joaat: {
				return eProtectionState::BlockAndNotify;
			} break;
			}
			return eProtectionState::Disabled;
		}
	private:
		typedValue m_value{};
	};
	class variadicFeature : public abstractFeature {
	public:
		variadicFeature(std::string id, std::string name, std::string description, std::vector<typedValue> values, fnptr<void(variadicFeature*)> callback, bool looped) :
			abstractFeature(id, name, description, {}, eFeatureType::VariadicFeature, looped),
			m_callback(callback), m_values(values), m_loop(looped) {
		}
		variadicFeature(std::string id, std::string name, std::vector<typedValue> values, fnptr<void(variadicFeature*)> callback, bool looped) :
			variadicFeature(id, name, {}, values, callback, looped) {
		}
		void init() override {
			m_looped = m_loop;
			for (auto& v : m_values) {
				push_value(v);
			}
			abstractFeature::init();
		}
		void run() override {
			m_callback(this);
			abstractFeature::run();
		}
		bool looped() {
			return m_loop && m_looped;
		}
	private:
		fnptr<void(variadicFeature*)> m_callback{};
		std::vector<typedValue> m_values{};
		bool m_loop{};
	};
}