#pragma once
#include <cstdint>
#include <vector>
#include <winnt.h>
#include <libloaderapi.h>

inline uint64_t scan(std::vector<uint16_t> bytes, const char* module = NULL) {
	uint64_t base{ reinterpret_cast<uint64_t>(GetModuleHandleA(module)) };
	PIMAGE_DOS_HEADER dosHeader{ reinterpret_cast<PIMAGE_DOS_HEADER>(base) };
	PIMAGE_NT_HEADERS ntHeader{ reinterpret_cast<PIMAGE_NT_HEADERS>(base + dosHeader->e_lfanew) };
	uint64_t size{ ntHeader->OptionalHeader.SizeOfImage };
	auto cb{ [](uint8_t* t, uint16_t* s, uint64_t l) -> bool {
		for (uint64_t i{}; i != l; ++i) {
			if (s[i] != 0x100 && s[i] != t[i]) {
				return false;
			}
		}
		return true;
	} };
	for (uint64_t i{ base }; i != size - bytes.size(); ++i) {
		if (cb(reinterpret_cast<uint8_t*>(i), bytes.data(), bytes.size())) {
			return i;
		}
	}
	return NULL;
}
inline uint64_t readInstruction(uint64_t address, uint8_t size, uint8_t length) {
	return address + *(int32_t*)(address + size) + length;
}