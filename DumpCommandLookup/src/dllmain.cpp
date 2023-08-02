#include "pch.h"
#include "table.h"
#include "lookup.h"

uint64_t getOldNativeFromTable(uint64_t hash) {
    for (auto& p : g_table) {
        if (p.u == hash || p.o == hash) {
            return p.o;
        }
    }
    return NULL;
}
struct gameNamespace {
    size_t m_index{};
    std::vector<uint64_t> m_natives{};
    std::vector<gameNamespaceLookup> m_map{};
    void buildMap() {
        if (m_index == 0) {
            for (uint64_t i{}; i != m_natives.size(); ++i) {
                uint64_t hash{ m_natives[i] };
                m_map.push_back({ i, hash });
            }
        }
        else {
            for (uint64_t i{}; i != m_natives.size(); ++i) {
                uint64_t hash{ m_natives[i] };
                if (uint64_t oldHash{ getOldNativeFromTable(hash) }) {
                    m_map.push_back({ i, oldHash });
                }
                else {
                    std::cout << "// " << std::uppercase << std::hex << "0x" << hash << " is not in the table!" << std::endl;
                }
            }
        }
    }
};
std::vector<gameNamespace> g_natives{};

bool checkBytes(uint64_t address, const std::vector<uint8_t>& bytes) {
    uint8_t* gameBytes{ reinterpret_cast<uint8_t*>(address) };
    for (size_t i{}; i != bytes.size(); ++i) {
        if (gameBytes[i] != bytes[i]) {
            return false;
        }
    }
    return true;
}
void processNamespace(uint64_t address) {
    //48 8D 05
    if (!checkBytes(address, { 0x48, 0x8D, 0x05 })) {
        return;
    }
    address = readInstruction(address, 3, 7);
    std::vector<uint64_t> natives{};
    //C2 00 00
    if (checkBytes(address, { 0xC2, 0x00, 0x00 })) {
        return;
    }
    for (uint64_t addr{ address }; !checkBytes(addr, { 0x48, 0x87, 0x2C });) {
        //48 8D 0D - lea rcx, [addr]
        if (checkBytes(addr, { 0x48, 0x8D, 0x0D })) {
            addr += 7;
            continue;
        }
        //48 8D 15 ? ? ? ?
        if (checkBytes(addr, { 0x48, 0x8D, 0x15 })) {
            addr += 7;
            continue;
        }
        //E8
        if (checkBytes(addr, { 0xE8 })) {
            addr += 5;
            continue;
        }
        //E9
        if (checkBytes(addr, { 0xE9 })) {
            addr = readInstruction(addr, 1, 5);
            continue;
        }
        //48 B9 - mov rcx [hash]
        if (checkBytes(addr, { 0x48, 0xB9 })) {
            addr += 2;
            uint64_t hash{ *reinterpret_cast<uint64_t*>(addr) };
            natives.push_back(hash);
            addr += 8;
            continue;
        }
        addr++;
    }
    //std::cout << "//Namespace " << g_natives.size() << " had " << natives.size() << " natives." << std::endl;
    g_natives.push_back({ g_natives.size(), std::move(natives) });
}
void dumpNatives(uint64_t address, uint64_t size) {
    for (uint64_t i{}; i != size; ++i) {
        processNamespace(address + i);
    }
}
DWORD WINAPI routine(LPVOID hmod) {
    if (!AttachConsole(GetCurrentProcessId()))
        AllocConsole();
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    uint64_t baseAddress{ reinterpret_cast<uint64_t>(g_main) };
    uint64_t scanAddr{ scan({ 0x48, 0x89, 0x5C, 0x24, 0x08, 0x48, 0x89, 0x6C, 0x24, 0x10, 0x48, 0x89, 0x74, 0x24, 0x18, 0x57, 0x48, 0x83, 0xEC, 0x20, 0xBA, 0x10, 0x00, 0x00, 0x00, 0xB9, 0x20, 0x03, 0x00, 0x00 }) };
    uint64_t address{ readInstruction(scanAddr + 0x1E, 1, 5) };
    uint64_t functionLength{ 0x32A };
    gameNamespace system{
        0,
        {
            0x4EDE34FBADD967A6,
            0xE81651AD79516E48,
            0xB8BA7F44DF1575E1,
            0xEB1C67C3A5333A92,
            0xC4BB298BD441BE78,
            0x83666F9FB8FEBD4B,
            0xC1B1E9A034A63A62,
            0x5AE11BC36633DE4E,
            0x0000000050597EE2,
            0x0BADBFA3B172435F,
            0xD0FFB162F40A139C,
            0x71D93B57D07F9804,
            0xE3621CC40F31FE2E,
            0xE816E655DE37FE20,
            0x652D2EEEF1D3E62C,
            0xA8CEACB4F35AE058,
            0x2A488C176D52CCA5,
            0xB7A628320EFF8E47,
            0xEDD95A39E5544DE8,
            0x97EF1E5BCE9DC075,
            0xF34EE736CF047844,
            0x11E019C8F43ACC8A,
            0xF2DB717A73826179,
            0xBBDA792448DB5A89,
            0x42B65DEEF2EDF2A1
        }
    };
    g_natives.push_back(system);
    std::cout << R"(#pragma once
#include <cstdint>

struct gameNamespaceLookup {
    uint64_t index{};
    uint64_t oldHash{};
    uint64_t newHash{};
};
)" << std::endl;
    dumpNatives(address, functionLength);
    size_t count{};
    for (auto& native : g_natives) {
        native.buildMap();
    }
    for (size_t i{}; i != g_natives.size(); ++i) {
        auto natives{ g_natives[i] };
        std::cout << std::dec << "inline gameNamespaceLookup g_namespace" << i << "Lookup[" << natives.m_map.size() << "]{" << std::endl;
        for (size_t j{}; j != natives.m_map.size(); ++j) {
            auto& pair{ natives.m_map[j] };
            std::cout << std::dec << "\t{ " << std::dec << pair.index << ", " << std::uppercase << std::hex << "0x" << pair.oldHash << " }," << std::endl;
            count++;
        }
        std::cout << "};" << std::endl;
    }
    std::cout << std::dec << "//Total number of natives: " << count << std::endl;
    while (g_running) {
        if (GetAsyncKeyState(VK_F12))
            g_running = false;
		std::this_thread::sleep_for(10ms);
	}

	fclose(stdout);
	FreeConsole();
	FreeLibraryAndExitThread(HMODULE(hmod), 0);
    return 0;
}
BOOL APIENTRY DllMain(HMODULE hmod, DWORD  reason, LPVOID) {
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
       CreateThread(nullptr, NULL, &routine, hmod, NULL, nullptr);
    } break;
    case DLL_PROCESS_DETACH: {
		g_running = false;
    } break;
    }
    return TRUE;
}