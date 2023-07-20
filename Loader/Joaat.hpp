#pragma once
#include "Pch.hpp"

namespace rage {
	using joaat_t = uint32_t;
	inline constexpr char tolower(char c) {
		return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
	}
	inline constexpr char toUpper(char c) {
		return (c >= 'a' && c <= 'z') ? c + ('A' - 'a') : c;
	}
	inline uint32_t joaat(std::string_view str) {
		uint32_t hash = 0;
		for (auto c : str)
			hash += tolower(c), hash += (hash << 10), hash ^= (hash >> 6);
		hash += (hash << 3), hash ^= (hash >> 11), hash += (hash << 15);
		return hash;
	}
}
template <typename string_view_t = std::string_view>
inline consteval uint32_t constexprJoaat(string_view_t str, bool const forceLowerCase = true) {
	uint32_t hash{};
	while (*str != '\0') {
		hash += forceLowerCase ? rage::tolower(*str) : *str, ++str;
		hash += hash << 10, hash ^= hash >> 6;
	}
	hash += (hash << 3), hash ^= (hash >> 11), hash += (hash << 15);

	return hash;
}
template <size_t arraySize>
inline consteval uint32_t joaatFromCharArray(char const(&str)[arraySize]) {
	uint32_t hash{};
	for (int i{}; i != arraySize; ++i) {
		hash += rage::tolower(str[i]);
		hash += hash << 10, hash ^= hash >> 6;
	}
	hash += (hash << 3), hash ^= (hash >> 11), hash += (hash << 15);

	return hash;
}

inline consteval uint32_t operator ""_joaat(const char* str, size_t) { return constexprJoaat(str); }
