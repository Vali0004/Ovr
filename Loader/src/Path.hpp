#pragma once
#include "Pch.hpp"

namespace Path {
	inline fs::path g_Path{};
	inline char g_PathBuffer[MAX_PATH]{};
	inline char g_DllBuffer[MAX_PATH]{};
	inline void Init() {
		strncpy(g_PathBuffer, BRAND"Path", MAX_PATH);
		strncpy(g_DllBuffer, BRAND".dll", MAX_PATH);
		fs::path Path{ std::getenv("appdata") };
		Path /= BRAND;
		if (!std::filesystem::exists(Path)) {
			std::filesystem::create_directories(Path);
			std::filesystem::create_directories(fs::path(Path).append("Modules"));
			std::filesystem::create_directories(fs::path(Path).append("Scripts"));
			std::filesystem::create_directories(fs::path(Path).append("Sounds"));
		}
		Path /= "Cache";
		if (std::filesystem::exists(Path)) {
			Path /= "pathdata.file";
			std::ifstream File(Path);
			std::string Content((std::istreambuf_iterator<char>(File)), (std::istreambuf_iterator<char>()));
			File.close();
			if (!Content.empty()) {
				std::string String{ Content.substr(0, Content.find("__")) };
				if (size_t Result{ String.find("%appdata%") }; Result != std::string::npos) {
					String = std::string(std::getenv("appdata")) + String.substr(sizeof("%appdata%"));
					strcpy(g_PathBuffer, String.c_str());
				}
				else {
					strcpy(g_PathBuffer, String.c_str());
				}
				strcpy(g_DllBuffer, Content.substr(Content.find("__") + 2).c_str());
				g_Path /= g_PathBuffer;
				g_Path /= g_DllBuffer;
			}
		}
		else {
			std::filesystem::create_directories(Path);
			std::ofstream File(Path);
			g_Path = fs::path();
			g_Path /= std::getenv("appdata");
			g_Path /= BRAND;
			g_Path /= "Dll";
			g_Path /= BRAND".dll";
			File << "%appata%" << "\\" << BRAND << "\\Dll" << "__" << BRAND << ".dll";
			File.close();
		}
	}
	inline void Set() {
		std::string Str{ g_PathBuffer };
		std::string FinalStr{ Str };
		if (!Str.empty()) {
			if (!Str.compare(BRAND "Path")) {
				g_Path = fs::path();
				g_Path /= std::getenv("appdata");
				g_Path /= BRAND;
				g_Path /= "Dll";
				g_Path /= BRAND".dll";
				return;
			}
			if (auto findRes{ Str.find("%appdata%") }; findRes != std::string::npos) {
				FinalStr = Str.substr(sizeof("%appdata%"));
				g_Path = fs::path(std::getenv("appdata")).append(FinalStr);
			}
			else {
				g_Path = fs::path(FinalStr);
			}
			Str = g_DllBuffer;
			if (!Str.empty()) {
				if (auto findRes{ Str.find(".dll") }; findRes == std::string::npos) {
					g_Path /= Str + ".dll";
				}
				else {
					g_Path /= Str;
				}
			}
			ofstream file(g_Path);
			file << g_PathBuffer << "__" << g_DllBuffer;
			file.close();
		}
	}
	inline void Open() {
		cmd(g_PathBuffer, true);
	}
}