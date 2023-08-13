#pragma once
#include "pch/pch.h"
#include "mem.h"
#include "module.h"
#include "exceptions/handler.h"

inline u16 g_foundSigCount{};
inline u16 g_totalSigCount{};
inline u16 g_failedSigCount{};
inline std::optional<u8> charToHex(cc c) {
	if (c >= 'a' && c <= 'f')
		return static_cast<u8>(static_cast<i32>(c) - 87);
	if (c >= 'A' && c <= 'F')
		return static_cast<u8>(static_cast<i32>(c) - 55);
	if (c >= '0' && c <= '9')
		return static_cast<u8>(static_cast<i32>(c) - 48);
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
inline bool doesMemoryMatch(u8* target, std::optional<u8> const* sig, u64 len) {
	for (u64 i{}; i != len; ++i) {
		if (sig[i] && *sig[i] != target[i]) {
			return false;
		}
	}
	return true;
}
inline u64 findPatternBruteforce(std::vector<std::optional<u8>> bytes, hmodule module = {}) {
	mem memoryRegion{ module.begin() };
	//We can skip the PE header and entry point
	for (u64 data{ memoryRegion + 0x1100ui64 }; data != module.size(); data++) {
		mem currentMemoryAddress{ memoryRegion.add(data) };
		if (doesMemoryMatch(currentMemoryAddress.as<u8*>(), bytes.data(), bytes.size())) {
			return currentMemoryAddress.as<u64>();
		}
	}
	return NULL;
}
inline u64 findPatternBoyerMooreHorspool(std::vector<std::optional<u8>> bytes, hmodule module = {}) {
	i64 maxShift{ static_cast<i64>(bytes.size()) }; //signed unsigned bullshit...
	i64 maxIdx{ maxShift - 1 };
	//Get wildcard index, and store max shifable byte count
	i64 wildCardIdx{ -1 };
	for (i64 i{ maxIdx - 1 }; i >= 0; --i) {
		if (!bytes[i]) {
			maxShift = maxIdx - i;
			wildCardIdx = i;
			break;
		}
	}
	//Store max shiftable bytes for non wildcards.
	i64 shiftTable[UINT8_MAX + 1]{};
	for (i64 i{}; i <= UINT8_MAX; ++i)
		shiftTable[i] = maxShift;
	for (i64 i{ wildCardIdx + 1 }; i != maxIdx; ++i)
		shiftTable[*bytes[i]] = maxIdx - i;
	//Loop data
	LAZY_FIX({
		for (i64 curIdx{}; curIdx != module.size() - static_cast<i64>(bytes.size());) {
			for (i64 sigIdx = maxIdx; sigIdx >= 0; --sigIdx) { //bruh. iLl dEfIneD fOr LoOp. Suck my motherfucking dick.
				if (bytes[sigIdx].has_value() && *module.begin().add(curIdx + sigIdx).as<u8*>() != bytes[sigIdx].value()) {
					curIdx += shiftTable[*module.begin().add(curIdx + maxIdx).as<u8*>()];
					break;
				}
				else if (sigIdx == 0) {
					return module.begin().add(curIdx).as<u64>();
				}
			}
		}
	});
	return NULL;
}
class scanner {
public:
	scanner(std::string name, std::string pattern, hmodule module = {}) :
		m_name(name), m_pattern(pattern), m_module(module), m_elements(createBytesFromString(m_pattern))
	{
	}
public:
	mem get() {
		g_totalSigCount++;
		try {
			mem res{ findPatternBoyerMooreHorspool(m_elements, m_module) };
			if (res) {
				g_foundSigCount++;
				LOG_DEBUG("Found {} at GTA5.exe+0x{:X}", m_name, res.as<u64>() - m_module.begin().as<u64>());
			}
			else {
				g_failedSigCount++;
				LOG(Info, "Failed to find {}", m_name);
			}
			return res;
		}
		catch (std::exception& ex) {
			g_failedSigCount++;
			LOG(Fatal, "Failed to find {} (Exception {})", m_name, ex.what());
		}
		return {};
	}
	mem getTry() {
		g_totalSigCount++;
		LAZY_FIX({
			mem res{ findPatternBruteforce(m_elements, m_module) };
			if (res) {
				LOG_DEBUG("Found {} at GTA5.exe+0x{:X}", m_name, res.as<u64>() - m_module.begin().as<u64>());
			}
			return res;
		});
		return {};
	}
private:
	std::string m_name{};
	std::string m_pattern{};
	hmodule m_module{};
	std::vector<std::optional<u8>> m_elements{};
};
inline mem scan(std::string key, std::string ptr, hmodule module = {}) {
	return scanner(key, ptr, module).get();
}
inline mem scanTry(std::string key, std::string ptr, hmodule module = {}) { //This will not report ANY errors. It is designed to silently fail for patches
	return scanner(key, ptr, module).getTry();
}
inline std::vector<mem> getAllResults(std::string ptr, hmodule module = {}) {
	std::vector<std::optional<u8>> bytes{ createBytesFromString(ptr) };
	std::vector<mem> results{};
	for (u64 i{}; i != module.size() - bytes.size(); ++i) {
		if (doesMemoryMatch(module.begin().add(i).as<u8*>(), bytes.data(), bytes.size())) {
			results.push_back(module.begin().add(i));
		}
	}
	return results;
}