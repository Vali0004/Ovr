#pragma once
#include "Pch.hpp"
#include "Threading.hpp"
#include "Path.hpp"
#include "Process.hpp"
#include "Registry.hpp"

namespace Game {
	inline constexpr std::array<std::string_view, 3> g_GameVersions{
		"Rockstar Games Launcher",
		"Steam",
		"Epic Games"
	};
	inline bool g_InjectOnLaunch{};
	inline int g_InjectionDelay{};
	inline bool g_NoFail{};
	inline int g_Type{};
	inline bool Open() {
		return FindWindowA("grcWindow", nullptr);
	}
	inline std::string VersionString() {
		return g_GameVersions[g_Type].data();
	}
	inline void WriteVersionData() {
		fs::path Path{ std::getenv("appdata") };
		Path /= BRAND;
		Path /= "Cache";
		Path /= "versiondata.file";
		std::ofstream File{ Path };
		File << VersionString() << "__" << g_Type;
		File.close();
	}
	inline int TypeFromModule() {
		if (Process::GetModule("steam_api64.dll")) {
			return 2;
		}
		else if (Process::GetModule("eossdk-win64-shipping.dll")) {
			return 1;
		}
		else {
			return 0;
		}
	}
	inline void Init() {
		fs::path Path{ std::getenv("appdata") };
		Path /= BRAND;
		Path /= "Cache";
		if (fs::exists(Path)) {
			Path /= "versiondata.file";
			std::ifstream File{ Path };
			std::string Content((std::istreambuf_iterator<char>(File)), (std::istreambuf_iterator<char>()));
			File.close();
			if (!Content.empty()) {
				size_t Seperator{ Content.find("__") };
				std::string Type{ Content.substr(0, Seperator) };
				g_Type = stoi(Content.substr(Seperator + 2));
				LOG(Info, "Detected your last used game version as {}", Type);
			}
		}
		else {
			fs::create_directories(Path);
			Path /= "versiondata.file";
			if (Open()) {
				g_Type = TypeFromModule();
				std::ofstream File{ Path };
				File << VersionString() << "__" << g_Type;
				File.close();
			}
		}
	}
	inline void Inject() {
		fs::path Directory(fs::path(std::getenv("appdata")).append(BRAND"\\Cache\\Dll"));
		if (!fs::exists(Directory))
			std::filesystem::create_directories(Directory);
		if (std::filesystem::exists(Path::g_Path) && !std::filesystem::is_empty(Path::g_Path)) {
			if (std::filesystem::exists(Directory)) {
				srand(time(NULL));
				std::filesystem::copy(Path::g_Path.string(), Directory.append(BRAND "_" + std::to_string(rand()) + ".dll").string(), std::filesystem::copy_options::overwrite_existing);
			}
			else {
				std::filesystem::create_directories(Directory);
				std::filesystem::copy(Path::g_Path.string(), Directory.string(), std::filesystem::copy_options::overwrite_existing);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(g_InjectionDelay));
			Process::Load(Directory);
		}
	}
	inline void OnFind() {
		if (!g_NoFail && Open()) {
			g_NoFail = true;
		}
	}
	inline void LookForGameCallback() {
		bool Running{ true };
		std::chrono::time_point Start{ std::chrono::steady_clock::now() };
		while (!Open() && Running) {
			std::chrono::seconds Elapsed{ std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - Start) };
			if (!g_NoFail && Elapsed.count() >= 27) {
				Running = false;
				LOG(Info, "Failed to launch GTA5 using {}", VersionString());
			}
			std::this_thread::sleep_for(1ms);
		}
		if (g_InjectOnLaunch)
			Inject();
	}
	inline bool LaunchProcess() {
		switch (g_Type) {
		case 0: {
			HKEY Key{ Registry::ReadKey(HK_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Rockstar Games\\Launcher", 0, READ) };
			if (Key) {
				const char* RGSPath{ (char*)Registry::ReadValue(Key, "InstallFolder", REG_SZ) };
				if (RGSPath) {
					cmd(R"("{}\\Launcher.exe" {})", false, RGSPath, "-minmodeApp=gta5");
				}
				else {
					LOG(Info, "Failed to launch GTA using {}", VersionString());
					g_Type++;
					return false;
				}
			}
			else {
				LOG(Info, "Failed to find {}", VersionString());
				g_Type++;
				return false;
			}
		} break;
		case 1: {
			int Result{ cmd("steam://rungameid/{}", true, "271590") };
			if (!Result) {
				LOG("Failed to launch GTA5 using {}", VersionString());
				g_Type++;
				return false;
			}
		} break;
		case 2: {
			int Result{ cmd("com.epicgames.launcher://apps/9d2d0eb64d5c44529cece33fe2a46482?{}&{}", true, "action=launch", "silent=true") };
			if (!Result) {
				LOG("Failed to launch GTA5 using {}", VersionString());
				g_Type = 0;
				return false;
			}
		} break;
		}
		return true;
	}
	inline void Launch() {
		LaunchProcess();
		g_NoFail = false;
		g_Threading.Queue(&LookForGameCallback);
	}
	inline void Close() {
		Process::Terminate(0, "grcWindow");
	}
	inline void Restart() {
		Close();
		std::this_thread::sleep_for(200ms);
		Launch();
	}
}