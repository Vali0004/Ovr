#pragma once
//Windows Includes (SDKs)
#pragma disable(warning : 4996)
#include <winsdkver.h>
#include <sdkddkver.h>
#include <windows.h>
#include <winternl.h>
#include <d3d11.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <cassert>
#include <cinttypes>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <type_traits>
#include <map>
#include <unordered_map>
#include <vector>
#include <array>
#include <wrl/client.h>
#include <random>
#include <shellapi.h>
//ImGui Includes
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/backends/imgui_impl_win32.h"
#include "ImGui/backends/imgui_impl_dx11.h"
//DirectX Includes
#include <dxgi.h>
#include <d3d11.h>
//Namespace Defines
using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;
//Ease Of Life Defines
using namespace std::chrono_literals;
namespace fs = std::filesystem;
template <typename type>
using ComPtr = Microsoft::WRL::ComPtr<type>;
template <typename t> requires std::is_function_v<t>
using FnPtr = t(*);