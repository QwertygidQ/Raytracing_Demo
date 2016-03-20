#include "Vector3.h"

#include <math.h>

	Vector3::Vector3()
	{
		x = y = z = 0;
	}

	Vector3::Vector3(const float arg)
	{
		x = y = z = arg;
	}

	Vector3::Vector3(const float xArg, const float yArg, const float zArg)
	{
		x = xArg;
		y = yArg;
		z = zArg;
	}

	float Vector3::length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector3& Vector3::normalize()
	{
		float len = length();
		if(len*len > 0)
			x /= len, y /= len, z /= len;
		return *this;
	}

	float Vector3::dot(const Vector3& vector) const
	{
		return x * vector.x + y * vector.y + z * vector.z;
	}

	Vector3 Vector3::operator+(const Vector3& vector) const
	{
		return Vector3(x + vector.x, y + vector.y, z + vector.z);
	}

	Vector3 Vector3::operator-(const Vector3& vector) const
	{
		return Vector3(x - vector.x, y - vector.y, z - vector.z);
	}

	Vector3 Vector3::operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	Vector3 Vector3::operator*(const Vector3& vector) const
	{
		return Vector3(x * vector.x, y * vector.y, z * vector.z);
	}

	Vector3 Vector3::operator*(const float multiplier) const
	{
		return Vector3(x * multiplier, y * multiplier, z * multiplier);
	}

	Vector3& Vector3::operator+=(const Vector3& vector)
	{
		x += vector.x, y += vector.y, z += vector.z;
		return *this;
	}

	Vector3& Vector3::operator*=(const Vector3& vector)
	{
		x *= vector.x, y *= vector.y, z *= vector.z;
		return *this;
	}
