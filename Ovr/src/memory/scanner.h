#pragma once
#include "pch/pch.h"
#include "mem.h"
#include "module.h"

inline std::optional<u8> charToHex(cc c) {
	if (c >= 'a' && c <= 'f')
		return static_cast<u8>(static_cast<s32>(c) - 87);
	if (c >= 'A' && c <= 'F')
		return static_cast<u8>(static_cast<s32>(c) - 55);
	if (c >= '0' && c <= '9')
		return static_cast<u8>(static_cast<s32>(c) - 48);
	return {};
}
inline std::vector<std::optional<u8>> createBytesFromString(std::string ptr) {
	std::vector<std::optional<u8>> bytes{};
	for (size_t i{}; i != ptr.size() - 1; ++i) {
		if (ptr[i] == ' ')
			continue;
		if (ptr[i] != '?') {
			if (auto c = charToHex(ptr[i]); c) {
				if (auto c2 = charToHex(ptr[i + 1]); c2)
					bytes.emplace_back(static_cast<uint8_t>((*c * 0x10) + *c2));
			}
		}
		else {
			bytes.push_back({});
		}
	}
	return bytes;
}
inline u64 findPatternBoyerMooreHorspool(std::vector<std::optional<u8>> bytes, hmodule module = {}) {
	u64 maxShift{ bytes.size() };
	u64 maxIdx{ maxShift - 1 };
	//Get wildcard index, and store max shifable byte count
	u64 wildCardIdx{ size_t(-1) };
	for (s32 i{ s32(maxIdx - 1) }; i >= 0; --i) {
		if (!bytes[i]) {
			maxShift = maxIdx - i;
			wildCardIdx = i;
			break;
		}
	}
	//Store max shiftable bytes for non wildcards.
	u64 shiftTable[UINT8_MAX + 1]{};
	for (u64 i{}; i <= UINT8_MAX; ++i)
		shiftTable[i] = maxShift;
	for (u64 i{ wildCardIdx + 1 }; i != maxIdx; ++i)
		shiftTable[*bytes[i]] = maxIdx - i;
	//Loop data
	for (size_t curIdx{}; curIdx != module.m_size - bytes.size();) {
		for (size_t sigIdx = maxIdx; sigIdx >= 0; --sigIdx) {
			if (bytes[sigIdx] && *module.m_begin.add(curIdx + sigIdx).as<u8*>() != *bytes[sigIdx]) {
				curIdx += shiftTable[*module.m_begin.add(curIdx + maxIdx).as<u8*>()];
				break;
			}
			else if (sigIdx == NULL) {
				return module.m_begin.add(curIdx).as<u64>();
			}
		}
	}
	return NULL;
}
struct scanner {
	scanner(std::string name, std::string pattern, hmodule mod = {}) :
		m_name(name), m_pattern(pattern), m_module(mod), m_elements(createBytesFromString(m_pattern))
	{
	}
	mem get() {
		mem res{ findPatternBoyerMooreHorspool(m_elements, m_module) };
		if (res) {
			LOG(FOREGROUND_INTENSITY, "Info", "Found {} at GTA5.exe+0x{:X}", m_name, res.as<u64>() - m_module.m_begin.as<u64>());
		}
		else {
			LOG(FOREGROUND_INTENSITY, "Info", "Failed to find {}", m_name);
		}
		return res;
	}
	std::string m_name{};
	std::string m_pattern{};
	hmodule m_module{};
	std::vector<std::optional<u8>> m_elements{};
};
inline mem scan(std::string key, std::string ptr, hmodule mod = {}) {
	return scanner(key, ptr, mod).get();
}