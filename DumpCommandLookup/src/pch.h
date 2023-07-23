#ifndef PCH_H
#define PCH_H

#include "framework.h"
#include "classes.h"
#include "memory.h"

inline uint32_t joaat(std::string_view str, bool forceLowerCase = true) {
	uint32_t hash{};
	for (char c : str) {
		hash += forceLowerCase ? tolower(c) : c;
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}
inline std::vector<std::string> getMatches(std::string str, std::string ex) {
    std::vector<std::string> ret{};
    std::regex expression{ ex };
    std::sregex_iterator iter{ str.begin(), str.end(), expression };
    std::sregex_iterator end{};
    while (iter != end) {
        ret.emplace_back((*iter).str());
        ++iter;
    }
    return ret;
}
inline std::string getFileContents(const std::filesystem::path& path) {
    std::ifstream file{ path };
    return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
}

inline HMODULE g_main{ GetModuleHandleA(NULL) };
inline bool g_running{ true };

#endif //PCH_H