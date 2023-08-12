#include "hooking/hooking.h"

i32 hooks::calculateMipLevel(u8 Type, u16 Width, u16 Height, u8 Levels, u32 Format) {
	i32 targetLevel{ CALL(calculateMipLevel, Type, Width, Height, Levels, Format) };
	i32 oldTargetLevel{ targetLevel };
	if (Levels > 1 && targetLevel < Levels) {
		std::string strName{ getCurrentStreamingName() };
		std::string baseName{ strName.substr(0, strName.find('.')) };
		u64 archetypeUnk{ 0xFFFFFFF };
		rage::fwArchetype* archetype{ pointers::g_getArchetype(rage::joaat(baseName), &archetypeUnk) };
		if (archetype) {
			u64 archetypeVtable{ *(u64*)archetype };
			if (archetypeVtable == pointers::g_vehicleModelInfoVtbl) {
				int limit{ (Format == 21 || Format == 32) ? 512 : 1024 };
				auto newWidth{ Width };
				auto newHeight{ Height };
				for (i32 i{}; i != targetLevel; ++i) {
					newWidth /= 2;
					newHeight /= 2;
				}
				while (newWidth > limit || newHeight > limit) {
					if (targetLevel >= Levels) {
						break;
					}
					newWidth /= 2;
					newHeight /= 2;
					targetLevel++;
				}
				if (targetLevel != oldTargetLevel) {
					LOG_DEBUG("Limited vehicle texture level in {} from{}x{} to{}x{}\n", strName, Width, Height, newWidth, newHeight);
				}
			}
			else if (archetypeVtable == pointers::g_pedModelInfoVtbl) {
				int limit{ 499 };
				auto newWidth{ Width };
				auto newHeight{ Height };
				for (i32 i{}; i != targetLevel; ++i) {
					newWidth /= 2;
					newHeight /= 2;
				}
				while (newWidth > limit || newHeight > limit) {
					if (targetLevel >= Levels) {
						break;
					}
					newWidth /= 2;
					newHeight /= 2;
					targetLevel++;
				}
				if (targetLevel != oldTargetLevel) {
					LOG_DEBUG("Limited ped texture level in {} from{}x{} to{}x{}\n", strName, Width, Height, newWidth, newHeight);
				}
			}
		}
	}
	return targetLevel;
}