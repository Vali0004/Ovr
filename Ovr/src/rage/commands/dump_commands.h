#pragma once
#include "pch/pch.h"
#include "conversion_table.h"
#include "memory/mem.h"

namespace util::game::commands {
    inline gameNamespaceLookup* getNamespaceFromIndex(u64 index) {
        switch (index) {
        case 0: { return g_namespace0Lookup; } break;
        case 1: { return g_namespace1Lookup; } break;
        case 2: { return g_namespace2Lookup; } break;
        case 3: { return g_namespace3Lookup; } break;
        case 4: { return g_namespace4Lookup; } break;
        case 5: { return g_namespace5Lookup; } break;
        case 6: { return g_namespace6Lookup; } break;
        case 7: { return g_namespace7Lookup; } break;
        case 8: { return g_namespace8Lookup; } break;
        case 9: { return g_namespace9Lookup; } break;
        case 10: { return g_namespace10Lookup; } break;
        case 11: { return g_namespace11Lookup; } break;
        case 12: { return g_namespace12Lookup; } break;
        case 13: { return g_namespace13Lookup; } break;
        case 14: { return g_namespace14Lookup; } break;
        case 15: { return g_namespace15Lookup; } break;
        case 16: { return g_namespace16Lookup; } break;
        case 17: { return g_namespace17Lookup; } break;
        case 18: { return g_namespace18Lookup; } break;
        case 19: { return g_namespace19Lookup; } break;
        case 20: { return g_namespace20Lookup; } break;
        case 21: { return g_namespace21Lookup; } break;
        case 22: { return g_namespace22Lookup; } break;
        case 23: { return g_namespace23Lookup; } break;
        case 24: { return g_namespace24Lookup; } break;
        case 25: { return g_namespace25Lookup; } break;
        case 26: { return g_namespace26Lookup; } break;
        case 27: { return g_namespace27Lookup; } break;
        case 28: { return g_namespace28Lookup; } break;
        case 29: { return g_namespace29Lookup; } break;
        case 30: { return g_namespace30Lookup; } break;
        case 31: { return g_namespace31Lookup; } break;
        case 32: { return g_namespace32Lookup; } break;
        case 33: { return g_namespace33Lookup; } break;
        case 34: { return g_namespace34Lookup; } break;
        case 35: { return g_namespace35Lookup; } break;
        case 36: { return g_namespace36Lookup; } break;
        case 37: { return g_namespace37Lookup; } break;
        case 38: { return g_namespace38Lookup; } break;
        case 39: { return g_namespace39Lookup; } break;
        case 40: { return g_namespace40Lookup; } break;
        case 41: { return g_namespace41Lookup; } break;
        case 42: { return g_namespace42Lookup; } break;
        case 43: { return g_namespace43Lookup; } break;
        case 44: { return g_namespace44Lookup; } break;
        default: { return g_namespace44Lookup; } break;
        }
    }
    struct gameNamespace {
        u64 m_index{};
        std::vector<u64> m_natives{};
        std::vector<gameNamespaceLookup> m_map{};
        void rebuildMap() {
            gameNamespaceLookup* gameNamespaceLookup{ getNamespaceFromIndex(m_index) };
            for (u64 i{}; i != m_natives.size(); ++i) {
                u64 hash{ m_natives[i] };
                gameNamespaceLookup[i].newHash = hash;
                m_map.push_back(std::move(gameNamespaceLookup[i]));
            }
            m_natives.clear();
        }
    };
    inline std::vector<gameNamespace> g_namespaces{};
    inline std::vector<gameNamespaceLookup> g_map{};
    inline u64 readInstruction(u64 address, u8 size, u8 length) {
        return address + *(i32*)(address + size) + length;
    }
    extern bool checkBytes(u64 address, const std::vector<u8>& bytes);
    extern void processNamespace(u64 address);
    extern void dump(u64 address, u64 size);
    extern gameNamespaceLookup getLookupFromHash(u64 hash);
    extern void intialize();
}