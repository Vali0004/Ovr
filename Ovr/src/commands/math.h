#pragma once
#include "pch/pch.h"
#include "rage/classes.h"

namespace math {
	inline float degreesToRadians(float degrees) {
		//Formula: x * PI/180 = y rad
		return static_cast<float>((static_cast<long double>(degrees) * PI) / 180.L);
	}
	inline float distance3D(Vector3 a, Vector3 b) {
		return (float)sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
	}
	inline Vector3 rotationToDirection(const Vector3 rot) {
		float xRadian{ degreesToRadians(rot.x) };
		float zRadian{ degreesToRadians(rot.z) };
		float absoluteXRadian{ fabs(cos(xRadian)) };
		return { (-sin(zRadian)) * absoluteXRadian, cos(zRadian) * absoluteXRadian, sin(xRadian) };
	}
	inline float distanceBetweenVecs(Vector3 a, Vector3 b) {
		return float(sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2)));
	}
}