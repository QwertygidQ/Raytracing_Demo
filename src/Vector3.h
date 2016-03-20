#pragma once

struct Vector3
{
	float x, y, z;

	Vector3();
	Vector3(const float arg);
	Vector3(const float xArg, const float yArg, const float zArg);

	float length() const;
	Vector3& normalize();

	float dot(const Vector3& vector) const;

	Vector3 operator+(const Vector3& vector) const;
	Vector3 operator-(const Vector3& vector) const;
	Vector3 operator-() const;
	Vector3 operator*(const Vector3& vector) const;
	Vector3 operator*(const float multiplier) const;
	Vector3& operator+=(const Vector3& vector);
	Vector3& operator*=(const Vector3& vector);
};
