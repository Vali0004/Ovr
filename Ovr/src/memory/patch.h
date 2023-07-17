#pragma once
#include "pch/pch.h"
#include "jitasm/jitasm.h"

class patch {
public:
	patch(ccp id, u8* ptr, const std::vector<u8>& bytes, bool set = false) : m_id(id), m_ptr(ptr), m_original(bytes), m_bytes(bytes), m_set(set)  {
		memcpy(m_original.data(), m_ptr, m_bytes.size());
	}
	~patch() {
		restore();
	}
public:
	void apply() {
		memcpy(m_ptr, m_bytes.data(), m_bytes.size());
	}
	void restore() {
		memcpy(m_ptr, m_original.data(), m_original.size());
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
	u8* m_ptr{};
	std::vector<u8> m_bytes{};
	std::vector<u8> m_original{};
	bool m_set{};
};
class patches {
public:
	template <typename t>
	void add(ccp id, t* ptr, std::vector<t> bytes, bool apply = true) {
		m_patches.push_back(MakeSmartPointer<patch>(id, ptr, bytes));
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
	std::vector<SmartPointer<patch>> m_patches{};
};
inline patches g_patches{};
class arxPatches {
public:
	void addInteg(u8* ptr) {
		const std::vector<u8> bytes{ 0x90ui8, 0x90ui8, 0x90ui8, 0x90ui8, 0x90ui8, 0x90ui8, 0x90ui8 };
		m_integCheckPatches.push_back(new patch(NULL, ptr, bytes, true));
		m_integCheckPatches.back()->apply();
	}
	void addCodeHealer(u8* ptr) {

	}
	void clear() {
		for (auto& p : m_integCheckPatches) {
			p->restore();
		}
		for (auto& p : m_codeHealerPatches) {
			p->restore();
		}
		m_integCheckPatches.clear();
		m_codeHealerPatches.clear();
	}
	size_t count() {
		return integCount() + codeHealCount();
	}
	size_t integCount() {
		return m_integCheckPatches.size();
	}
	size_t codeHealCount() {
		return m_codeHealerPatches.size();
	}
private:
	std::vector<patch*> m_integCheckPatches{};
	std::vector<patch*> m_codeHealerPatches{};
};
inline arxPatches g_arxPatches{};
template <typename v, typename a>
inline void put(a address, v value) {
	memcpy((void*)address, &value, sizeof(value));
}