#pragma once
#include "abstract.h"

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