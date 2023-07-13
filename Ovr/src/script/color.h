#pragma once
#include "pch/pch.h"

namespace elements {
	class color {
	public:
		color(u8 r, u8 g, u8 b, u8 a) : r(r), g(g), b(b), a(a) {}
		color(u8 r, u8 g, u8 b) : color(r, g, b, 255) {}
		color(u32 packed) {
			unpack(packed);
		}
	public:
		u8 r{}, g{}, b{}, a{};
		u32 pack() { return a << 24 | b << 16 | g << 8 | r << 0; }
		void unpack(u32 value) {
			r = (value >> 0) & 0xFF;
			g = (value >> 8) & 0xFF;
			b = (value >> 16) & 0xFF;
			a = (value >> 24) & 0xFF;
		}
	};
}