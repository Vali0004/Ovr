#pragma once
#include "pch/pch.h"

class VFTFunctionData {
public:
	VFTFunctionData(void* fn, size_t fnIdx) : m_function(fn), m_functionIndex(fnIdx) {}
public:
	auto getFunction() {
		return uintptr_t(m_function);
	}
	auto getFunctionIndex() {
		return m_functionIndex + 1;
	}
public:
	void* m_function{};
	u64 m_functionIndex{};
};
class VFT {
public:
	VFT(VFTFunctionData* vft, u64 vftSize) : m_vft(vft), m_vftSize(vftSize) {}
	VFT(std::vector<VFTFunctionData> vft) : m_vft(vft.data()), m_vftSize(vft.size()) {}
	VFT(void* vft, u64 vftSize) : m_vft((VFTFunctionData*)vft), m_vftSize(vftSize) {}
public:
	auto getVft() {
		return m_vft;
	}
	auto getVftAsUint() {
		return reinterpret_cast<uintptr_t**>(getVft());
	}
	auto getVftSize() {
		return m_vftSize;
	}
public:
	VFTFunctionData* m_vft{};
	size_t m_vftSize{};
};
class hookVFT {
public:
	hookVFT(VFT vft, VFT replacementVft) :
		m_vft(vft.getVftAsUint()), m_vftSize(vft.getVftSize()),
		m_copyOfOrginialVft(*m_vft), m_tableOfReplaceFunctions(std::make_unique<uintptr_t[]>(m_vftSize + 1))
	{
		std::copy_n(m_copyOfOrginialVft - 1, m_vftSize + 1, m_tableOfReplaceFunctions.get());
		for (size_t i{}; i != replacementVft.getVftSize(); ++i) {
			hook(replacementVft.getVft()[i]);
		}
	}
public:
	void enable() {
		*m_vft = &m_tableOfReplaceFunctions[1];
	}
	void disable() {
		for (u64 i{}; i != m_vftSize; ++i) {
			if (m_tableOfReplaceFunctions[i + 1] != m_copyOfOrginialVft[i]) {
				m_tableOfReplaceFunctions[i + 1] = m_copyOfOrginialVft[i];
			}
		}
		*m_vft = m_copyOfOrginialVft;
	}
public:
	void hook(VFTFunctionData data) {
		m_tableOfReplaceFunctions[data.getFunctionIndex()] = data.getFunction();
	}
	void unhook(size_t index) {
		m_tableOfReplaceFunctions[index + 1] = m_copyOfOrginialVft[index];
	}
public:
	template <typename type>
	type getOg(size_t index) {
		return type(m_copyOfOrginialVft[index]);
	}
public:
	u64** m_vft;
	u64 m_vftSize;
public:
	u64* m_copyOfOrginialVft;
	std::unique_ptr<u64[]> m_tableOfReplaceFunctions;
};