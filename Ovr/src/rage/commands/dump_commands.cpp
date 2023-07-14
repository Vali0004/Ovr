#include "dump_commands.h"
#include "memory/pointers.h"

namespace util::game::commands {
    bool checkBytes(u64 address, const std::vector<u8>& bytes) {
        u8* gameBytes{ reinterpret_cast<u8*>(address) };
        for (u64 i{}; i != bytes.size(); ++i) {
            if (gameBytes[i] != bytes[i]) {
                return false;
            }
        }
        return true;
    }
    void processNamespace(u64 address) {
        //48 8D 05
        if (!checkBytes(address, { 0x48, 0x8D, 0x05 })) {
            return;
        }
        address = readInstruction(address, 3, 7);
        std::vector<u64> commands{};
        //C2 00 00
        if (checkBytes(address, { 0xC2, 0x00, 0x00 })) {
            return;
        }
        for (u64 addr{ address }; !checkBytes(addr, { 0x48, 0x87, 0x2C });) {
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
                u64 hash{ *reinterpret_cast<u64*>(addr) };
                commands.push_back(hash);
                addr += 8;
                continue;
            }
            addr++;
        }
        g_namespaces.push_back({ g_namespaces.size(), std::move(commands) });
    }
    void dump(u64 address, u64 size) {
        for (uint64_t i{}; i != size; ++i) {
            processNamespace(address + i);
        }
    }
    gameNamespaceLookup getLookupFromHash(u64 hash) {
        for (auto& pair : g_map) {
            if (pair.oldHash == hash) {
                return pair;
            }
        }
        return { 0, hash, NULL };
    }
    void intialize() {
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
        g_namespaces.push_back(system);
        u64 functionLength{ 0x32A };
        dump(pointers::g_nativeRegistration, functionLength);
        for (auto& cmd : g_namespaces) {
            cmd.rebuildMap();
        }
        for (u64 i{}; i != g_namespaces.size(); ++i) {
            auto cmd{ g_namespaces[i] };
            for (u64 j{}; j != cmd.m_map.size(); ++j) {
                auto& pair{ cmd.m_map[j] };
                g_map.push_back(pair);
            }
        }
        g_namespaces.clear();
        LOG(Info, "Parsed {} native commands from registry", g_map.size());
    }
}