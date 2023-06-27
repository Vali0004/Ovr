#pragma once
#include "pch/pch.h"

namespace elements {
	struct color {
		u8 r{}, g{}, b{}, a{};
		inline u32 pack() { return a << 24 | b << 16 | g << 8 | r << 0; }
	};
}