#pragma once
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "src\\curl\\libcurl.lib")

#include "framework.h"

#define FOREGROUND_WHITE FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define LOG(c, t, fmt, ...) \
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<uint16_t>(c)); \
	std::cout << t << " | " << std::vformat(fmt, std::make_format_args(__VA_ARGS__)) << std::endl;
#define REGISTER_LOG(n) \
	if (!AttachConsole(GetCurrentProcessId())) \
		AllocConsole(); \
	SetConsoleTitleA(n); \
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#define DESTROY_LOG() \
	FreeConsole(); \
	fclose(stdout);
#define SIZEOF(a) sizeof(a) / sizeof(std::remove_pointer_t<decltype(a)>)
#define COUNT(a) ((sizeof(a)/sizeof(0[a])) / ((size_t)(!(sizeof(a) % sizeof(0[a])))))
#define PI 3.141592653589793238462643383279502884L
	
template <typename t>
using comPtr = Microsoft::WRL::ComPtr<t>;
template <typename t> requires std::is_function_v<t>
using fnptr = std::add_pointer_t<t>;
using u64 = uint64_t;
using s64 = int64_t;
using u32 = uint32_t;
using s32 = int32_t;
using u16 = uint16_t;
using s16 = int16_t;
using u8 = uint8_t;
using s8 = int8_t;
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
    inline auto g_splitStr = [](std::string str, char split) -> std::vector<std::string> {
        std::vector<std::string> fields{};
        std::string field{};
        for (int i = 0; i < str.length(); i++) {
            if (str.at(i) == split) {
                fields.push_back(field);
                field.erase();
            }
            else {
                field += str.at(i);
                if (i == str.length() - 1)
                    fields.push_back(field);
            }
        }
        return fields;
    };
}
using namespace defines;

namespace fs = std::filesystem;