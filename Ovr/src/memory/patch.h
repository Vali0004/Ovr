#pragma once
#include "pch/pch.h"

class patch {
public:
	patch(ccp id, u64* ptr, std::vector<i32> bytes) : m_id(id), m_ptr(ptr), m_original(bytes), m_bytes(bytes) {
		memcpy(m_original.data(), m_ptr, m_original.size());
	}
	~patch() {
		restore();
	}
public:
	void apply() {
		memcpy(m_ptr, m_bytes.data(), m_bytes.size());
		for (auto& b : m_bytes) {
			memset(m_ptr, b, sizeof(b));
		}
	}
	void restore() {
		for (auto& b : m_original) {
			memset(m_ptr, b, sizeof(b));
		}
	}
	template <typename t>
	t* getPtr() {
		return reinterpret_cast<t*>(m_ptr);
	}
	ccp& id() {
		return m_id;
	}
private:
	ccp m_id{};
	void* m_ptr{};
	std::vector<i32> m_bytes{};
	std::vector<i32> m_original{};
};
class patches {
public:
	template <typename t>
	void add(ccp id, t ptr, std::vector<i32> bytes, bool apply = true) {
		m_patches.push_back(std::make_unique<patch>(id, (i32*)ptr, bytes));
		if (apply)
			m_patches.back()->apply();
	}
	template <typename t>
	void remove(ccp id) {
		if (patch* p = get<t>(id); p) {
			p->resore();
		}
	}
	patch* get(ccp id) {
		for (auto& p : m_patches) {
			if (p->id() == id) {
				return p.get();
			}
		}
		return nullptr;
	}
	void reset() {
		for (auto& p : m_patches) {
			p->restore();
		}
		m_patches.clear();
	}
private:
	std::vector<std::unique_ptr<patch>> m_patches{};
};
inline patches g_patches{};