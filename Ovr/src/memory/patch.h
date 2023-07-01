#pragma once
#include "pch/pch.h"

class patch {
public:
	patch(ccp id, i32* ptr, const std::vector<i32>& bytes, bool set = false) : m_id(id), m_ptr(ptr), m_original(bytes), m_bytes(bytes), m_set(set)  {
		memcpy(m_original.data(), m_ptr, m_original.size());
	}
	~patch() {
		restore();
	}
public:
	void apply() {
		if (m_set) {
			memset(m_ptr, m_bytes.at(0), m_bytes.size());
		}
		else {
			memcpy(m_ptr, m_bytes.data(), m_bytes.size());
		}
	}
	void restore() {
		if (m_set) {
			for (auto& b : m_original) {
				memset(m_ptr, b, 1);
			}
		}
		else {
			memcpy(m_ptr, m_original.data(), m_original.size());
		}
	}
	template <typename t>
	t* get() {
		return reinterpret_cast<t*>(m_ptr);
	}
	ccp& id() {
		return m_id;
	}
private:
	ccp m_id{};
	i32* m_ptr{};
	std::vector<i32> m_bytes{};
	std::vector<i32> m_original{};
	bool m_set{};
};
class patches {
public:
	template <typename t>
	void add(ccp id, t* ptr, std::vector<t> bytes, bool apply = true) {
		m_patches.push_back(std::make_unique<patch>(id, ptr, bytes));
		if (apply)
			m_patches.back()->apply();
	}
	template <typename t>
	void remove(ccp id) {
		if (patch* p = get<t>(id); p) {
			p->restore();
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