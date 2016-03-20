#include "Sphere.h"
#include <math.h>

Sphere::Sphere(const Vector3& c, const float r, const Vector3& sc, const float refl, const float tr, const Vector3& ec)
{
	center = c;
	radius = r;

	surfaceColor = sc;

	reflection = refl;
	transparency = tr;

	emissionColor = ec;
}

Sphere::~Sphere()
{

}

bool Sphere::intersect(const Vector3& rayStart, const Vector3& rayDir, float& lenToPoint1, float& lenToPoint2) const
{
	Vector3 lenFromCenterToRayStart = center - rayStart;

	float a = lenFromCenterToRayStart.dot(rayDir);
	if(a < 0)
		return false;

	float b = lenFromCenterToRayStart.dot(lenFromCenterToRayStart) - a * a;
	if(b > radius * radius)
		return false;

	float c = sqrt(radius * radius - b);

	lenToPoint1 = a-c;
	lenToPoint2 = a+c;

	return true;

}
