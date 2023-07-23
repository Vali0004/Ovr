#pragma once
#include "Pch.hpp"

namespace Process {
	inline uint32_t GetId(const char* Class, const char* Name = NULL) {
		HWND Hwnd{ FindWindowA(Class, Name) };
		if (!Hwnd) {
			LOG(Fatal, "Failed to find {}", Class);
			g_Running = false;
			return NULL;
		}
		DWORD Pid{};
		GetWindowThreadProcessId(Hwnd, &Pid);
		return Pid;
	}
	inline HANDLE GetHandle(int Pid) {
		HANDLE Handle{ OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid) };
		if (!Handle) {
			LOG(Fatal, "Failed to open handle");
			g_Running = false;
			return nullptr;
		}
		return Handle;
	}
	inline HMODULE GetModule(const char* name) {
		uint32_t Pid{ GetId("grcWindow") };
		HANDLE Handle{ GetHandle(Pid) };
		HMODULE Modules[255]{};
		MODULEINFO ModuleInfo{};
		DWORD CallbackNeeded{};
		if (!K32EnumProcessModulesEx(Handle, Modules, sizeof(Modules), &CallbackNeeded, LIST_MODULES_64BIT)) {
			LOG(Fatal, "Failed to enumerate modules");
			g_Running = false;
			return nullptr;
		}
		for (int32_t i{}; i != (CallbackNeeded / sizeof(HMODULE)); ++i) {
			//Get the full path to the module's file
			char ModNameCharArray[MAX_PATH];
			if (K32GetModuleFileNameExA(Handle, Modules[i], ModNameCharArray, MAX_PATH)) {
				std::string String{ ModNameCharArray };
				if (!Modules[i]) {
					LOG(Fatal, "Module at index {} is null", i);
					g_Running = false;
					return nullptr;
				}
				if (!K32GetModuleInformation(Handle, Modules[i], &ModuleInfo, sizeof(ModuleInfo))) {
					LOG(Fatal, "Failed to get module info");
					g_Running = false;
					return nullptr;
				}
				if (String.compare(name)) {
					CloseHandle(Handle);
					return Modules[i];
				}
			}
		}
		if (Handle)
			CloseHandle(Handle);
		return nullptr;
	}
	inline void Load(fs::path FilePath) {
		HANDLE Handle{ GetHandle(GetId("grcWindow")) };
		LPVOID Mem{ VirtualAllocEx(Handle, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE) };
		if (!Mem) {
			return;
		}
		if (!WriteProcessMemory(Handle, Mem, FilePath.string().c_str(), MAX_PATH, nullptr)) {
			LOG(Fatal, "Failed to write dll to memory");
			g_Running = false;
			return;
		}
		HANDLE Thread{ CreateRemoteThread(Handle, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), Mem, NULL, nullptr) };
		if (!Thread) {
			LOG(Fatal, "Failed to create remote thread");
			g_Running = false;
			return;
		}
		CloseHandle(Handle);
		if (!Mem) {
			VirtualFreeEx(Handle, Mem, 0, MEM_RELEASE);
		}
	}
	inline void Terminate(int Code, const char* Class, const char* Name = NULL) {
		uint32_t Pid{ GetId(Class, Name) };
		HANDLE Handle{ GetHandle(Pid) };
		TerminateProcess(Handle, Code);
	}
}