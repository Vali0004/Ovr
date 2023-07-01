#pragma once
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "src\\curl\\libcurl.lib")

#include "framework.h"
#include "rage/enums.h"
#define BRAND "Ovr"
#define UPPER_BRAND "OVR"
#define LOWER_BRAND "ovr"
#define DEBUG

#define SIZEOF(a) sizeof(a) / sizeof(std::remove_pointer_t<decltype(a)>)
#define COUNT(a) ((sizeof(a)/sizeof(0[a])) / ((size_t)(!(sizeof(a) % sizeof(0[a])))))
#define ONCE(a) do a while (false)
#define PI 3.141592653589793238462643383279502884L
	
template <typename t>
using comPtr = Microsoft::WRL::ComPtr<t>;
template <typename t> requires std::is_function_v<t>
using fnptr = std::add_pointer_t<t>;
using u64 = unsigned long long;
using i64 = signed long long;
using u32 = unsigned int;
using i32 = signed int;
using u16 = unsigned short;
using i16 = signed short;
using u8 = unsigned char;
using i8 = signed char;
using cc = const char;
using ccp = cc*;
using fp = float;
using db = double;
class stackWalker : public StackWalker {
public:
	stackWalker() : StackWalker() {}
	stackWalker(ExceptType exType) : StackWalker(exType) {}
	virtual void OnOutput(LPCSTR szText);
	virtual void OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType, LPCSTR pdbName, ULONGLONG fileVersion);
	virtual void OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName);
	virtual void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr);
    virtual void OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry);
};
namespace defines {
	inline bool g_running{ true };
	inline HMODULE g_module{};
	inline HANDLE g_thread{};
	inline u16 g_selectedPlayer{};
	inline u32 g_selectedFriend{};
	inline std::string g_selectedAsi{};
	inline eSessionTypes g_sessionType{ eSessionTypes::Offline };
	inline bool debug() {
		#ifdef DEBUG
			return true;
		#endif
		return false;
	}
    inline bool isNumber(std::string str) {
        for (char const& c : str)
            if (std::isdigit(c) == 0)
                return false;
        return true;
    }
    inline bool containsANumber(std::string str) {
        for (char const& c : str)
            if (std::isdigit(c))
                return true;
        return false;
    }
	inline std::string gsTypeToString(eSessionTypes gstype) {
		switch (gstype) {
		case eSessionTypes::InviteOnly: { return "Invite-Only"; } break;
		case eSessionTypes::FriendsOnly: { return "Friends-Only"; } break;
		case eSessionTypes::CrewOnly: { return "Crew-Only"; } break;
		case eSessionTypes::CrewSession: { return "Crew"; } break;
		case eSessionTypes::Solo: { return "Solo"; } break;
		case eSessionTypes::Public: { return "Public"; } break;
		}
		return "Offline";
	}
    inline std::string lStr(std::string str) {
        std::string t{ str };
        std::transform(t.begin(), t.end(), t.data(), [](char c) { return tolower(c); });
        return t;
    }
    inline std::vector<std::string> lSArr(std::vector<std::string> sArr) {
        std::vector<std::string> tArr{ sArr };
        for (auto& str : tArr) {
            std::string t{ str };
            std::transform(t.begin(), t.end(), t.data(), [](char c) { return tolower(c); });
        }
        return tArr;
    }
    inline std::vector<std::string> getMatches(std::string str, std::string ex) {
        std::vector<std::string> matches{};
        std::regex expression{ ex };
        std::sregex_iterator iter{ str.begin(), str.end(), expression };
        std::sregex_iterator end{};
        while (iter != end) {
            matches.push_back((*iter).str());
            ++iter;
        }
        return matches;
    }
    inline std::vector<std::string> splitString(const std::string& string, char split) {
		std::vector<std::string> output{};
		u64 previousPosition{}, position{};
		while ((position = string.find(string, position)) != std::string::npos) {
			std::string substring{ string.substr(previousPosition, position - previousPosition) };
			output.push_back(substring);
			previousPosition = ++position;
		}
		output.push_back(string.substr(previousPosition, position - previousPosition));
		return output;
	}
}
using namespace defines;

namespace fs = std::filesystem;