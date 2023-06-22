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
using u64 = uint64_t;
using i64 = int64_t;
using u32 = uint32_t;
using i32 = int32_t;
using u16 = uint16_t;
using i16 = int16_t;
using u8 = uint8_t;
using i8 = int8_t;
using cc = const char;
using ccp = cc*;
using fp = float_t;
using db = double_t;
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
	inline eSessionTypes g_sessionType{};
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
		case eSessionTypes::Public: { return "Public"; } break;
		}
		return "Offline";
	}
    inline std::string lStr(std::string str) {
        std::string t{ str };
        std::transform(t.begin(), t.end(), t.data(), [](char c) { return tolower(c); });
        return t;
    }
    inline std::vector<std::string> splitString(std::string string, char split) {
        std::vector<std::string> strings{};
        std::string buffer{};
        for (u64 i{}; i != string.size(); ++i) {
            char& c{ string[i] };
            if (c == split) {
                strings.push_back(buffer);
                buffer.erase();
            }
            else {
                buffer += c;
                if (i == string.length() - 1) {
                    strings.push_back(buffer);
                }
            }
        }
        return strings;
    }
}
using namespace defines;

namespace fs = std::filesystem;