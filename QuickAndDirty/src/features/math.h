#pragma once
#include "pch/pch.h"
#include "rage/classes.h"

namespace math {
	inline float degToRad(const float deg) {
		auto rad = (3.14159265359 / 180) * deg;
		return (float)rad;
	}
	inline float distance3D(Vector3 a, Vector3 b) {
		return (float)sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
	}
	inline Vector3 rotToDir(const Vector3 rot) {
		auto xRadian = degToRad(rot.x);
		auto zRadian = degToRad(rot.z);
		auto num = abs(cos(xRadian));
		return { (-sin(zRadian)) * num, cos(zRadian) * num, sin(xRadian) };
	}
	inline float distanceBetweenVecs(Vector3 a, Vector3 b) {
		return float(sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2)));
	}
}