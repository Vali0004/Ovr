#pragma once
#include "memory/pointers.h"

constexpr uint64_t g_GB{ 1000 * 1024 * 1024 }; //Use 1000 for one so we catch 'hardware reserved' memory as well
inline int g_extRamMode{};
inline int* g_budgetScale{};
inline int* g_isStereo{};
inline void* allocateStub(u64 size) {
	return malloc(size); //cursed
}
inline bool g_budgetingInitFailed{};
inline u8 g_oldOpcode{};
inline i32 g_oldJmpLoc{};
inline i32 g_oldBudgetScale{};
inline i32 g_oldSteroCheckScale{};
inline mem g_loc{};
struct budgeting {
	static void init() {
		g_isStereo = (i32*)allocateStub(4);
		g_budgetScale = (i32*)allocateStub(4);
		*g_budgetScale = NULL;
		*g_isStereo = TRUE;
		mem loc{ scanTry("", "84 C0 0F 84 4B 01 00 00 0F B6").sub(0x46) };
		if (!loc || loc.add(0x101).as<u64>() == (u64)bigUpdate - g_loc.add(0x102).as<u64>() - 5) {
			g_budgetingInitFailed = true;
			return;
		}
		//Store location
		g_loc = loc;
		//Nop 6
		g_patches.add("", loc.add(0x48).as<u8*>(), { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, });
		//Store old values
		g_oldBudgetScale = *loc.add(0xA6).as<i32*>();
		g_oldSteroCheckScale = *loc.add(0xBA).as<i32*>();
		g_oldOpcode = *loc.add(0x101).as<u8*>();
		g_oldJmpLoc = *loc.add(0x102).as<i32*>();
		//Call
		g_loc.add(0x101).as<u8&>() = 0xE8;
		g_loc.add(0x102).as<i32&>() = (u64)bigUpdate - g_loc.add(0x102).as<u64>() - 5;
		//Put
		g_loc.add(0xA6).as<i32&>() = (u64)g_budgetScale - loc.add(0xA6).add(4).as<u64>();
		//Put
		g_loc.add(0xBA).as<i32&>() = (u64)g_isStereo - loc.add(0xBA).add(4).as<u64>();
	}
	static void restore() {
		g_loc.add(0xA6).as<i32&>() = g_oldBudgetScale;
		g_loc.add(0xBA).as<i32&>() = g_oldSteroCheckScale;
		g_loc.add(0x101).as<u8&>() = g_oldOpcode;
		g_loc.add(0x102).as<i32&>() = g_oldJmpLoc;
	}
	static void postInit() {
		if (!g_budgetingInitFailed) {
			return;
		}
		MEMORYSTATUSEX msex{};
		msex.dwLength = sizeof(msex);
		GlobalMemoryStatusEx(&msex);
		uint32_t allocatorReservation{};
		if (msex.ullTotalPhys >= 16 * g_GB) {
			allocatorReservation = 0x7FFFFFFF;
			g_extRamMode = 2;
		}
		else if (msex.ullTotalPhys >= 12 * g_GB) {
			allocatorReservation = 0x60000000;
			g_extRamMode = 1;
		}
		//The full code will 100% break 4/4GB systems
		if (g_extRamMode == 0) {
			setGamePhysicalBudget(3 * g_GB);
			return;
		}
		//Extend the pool a bit
		mem grcResourceCachePool{ scan("GRCP", "BA 00 00 05 00 48 8B C8 44 88").add(1) };
		grcResourceCachePool.as<u32&>() = 0xA0000;
		grcResourceCachePool.add(22).as<u32&>() = 0xA001B;
		//Increase allocator amount
		if (allocatorReservation) {
			*pointers::g_allocatorAmount = allocatorReservation;
		}
		setGamePhysicalBudget(3 * g_GB);
	}
	static void bigUpdate(int who, int what) {
		*g_budgetScale = what;
		setGamePhysicalBudget(0);
		pointers::g_updateVideoMemoryBar(0);
	}
	static void setGamePhysicalBudget(u64 budget) {
		static u64 baseBudget{};
		if (budget == 0) {
			budget = baseBudget;
		}
		else {
			baseBudget = budget;
		}
		float multiplier{ getBudgetMultiplier() };
		//This is designed to fix the logic error with low/high/veryhigh
		//The logic error/issue is R* seems to thought in a few cases that the texture setting flag is mapped to
		// normal, high, very high, and not unused, normal, high/very high.
		// This creates the issue where very high is just high with the hi texture flag enabled.
		//This will fix low and normal actually being low and normal
		for (i32 i{}; i != 80; i += 4) {
			u64 value{ static_cast<u64>(budget * multiplier) };
			pointers::g_vramLocation[i + 3] = value;
			pointers::g_vramLocation[i + 2] = value;
			pointers::g_vramLocation[i + 1] = static_cast<u64>(value / 1.5f);
			pointers::g_vramLocation[i] = static_cast<u64>(value / 2.f);
		}
	}
	static float getBudgetMultiplier() {
		return (*g_budgetScale / 12.f) + 1.f;
	}
};