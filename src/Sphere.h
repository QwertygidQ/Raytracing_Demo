#pragma once

#include "Vector3.h"

struct Sphere
{
	Vector3 center;
	float radius;

	Vector3 surfaceColor, emissionColor;
	float reflection, transparency;

	Sphere(const Vector3& c, const float r, const Vector3& sc, const float refl = 0, const float tr = 0, const Vector3& ec = 0);
	~Sphere();

	bool intersect(const Vector3& rayStart, const Vector3& rayDir, float& lenToPoint1, float& lenToPoint2) const;
};
