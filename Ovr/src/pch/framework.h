#pragma once
//Windows Includes (SDKs)
#include <winsdkver.h>
#include <sdkddkver.h>
#include <windows.h>
#include <winternl.h>
#include <d3d11.h>
#include <dxgi.h>
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
#include <filesystem>
#include <timeapi.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <regex>
#include <variant>
#include <Mmsystem.h>
//DirectXTK Includes
#include "directx/DirectXHelpers.h"
#include "directx/DDSTextureLoader.h"
#include "directx/CommonStates.h"
#include "directx/Effects.h"
#include "directx/GamePad.h"
#include "directx/GeometricPrimitive.h"
#include "directx/GraphicsMemory.h"
#include "directx/Keyboard.h"
#include "directx/Model.h"
#include "directx/Mouse.h"
#include "directx/PostProcess.h"
#include "directx/PrimitiveBatch.h"
#include "directx/ScreenGrab.h"
#include "directx/SimpleMath.h"
#include "directx/SpriteBatch.h"
#include "directx/SpriteFont.h"
#include "directx/VertexTypes.h"
#include "directx/WICTextureLoader.h"
//ImGui Includes
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "imgui/backends/imgui_impl_win32.h"
//cURL Includes
#define CURL_STATICLIB
#include <curl\curl.h>
//StackWalker Includes
#include "stackwalker/stackwalker.h"
//ImGui Defines
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//Namespace Defines
using namespace std::chrono;
using namespace std::chrono_literals;
//Shortened types
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
using fp = float;
using db = double;