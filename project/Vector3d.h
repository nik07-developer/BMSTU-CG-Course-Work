#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

struct Vector3d
{
	float x, y, z;
	Vector3d();
	Vector3d(float x, float y, float z);
	Vector3d(const Vector3d &base);

	float length() const;

	static Vector3d zero() { return Vector3d(0, 0, 0); }
	static Vector3d forward() { return Vector3d(0, 0, 1); }
	static Vector3d back() { return Vector3d(0, 0, -1); }
	static Vector3d right() { return Vector3d(1, 0, 0); }
	static Vector3d left() { return Vector3d(-1, 0, 0); }
	static Vector3d up() { return Vector3d(0, 1, 0); }
	static Vector3d down() { return Vector3d(0, -1, 0); }
	
	void normalize();
	Vector3d normalized() const;

	inline void rotateX(float rx);
	inline void rotateY(float ry);
	inline void rotateZ(float rz);
	void rotate(const Vector3d &rotor);
	void rotateBack(const Vector3d &rotor);

	void move(const Vector3d &offset);

	float dot(const Vector3d &arg) const;

	Vector3d operator+ (const Vector3d &arg) const;
	Vector3d operator- (const Vector3d &arg) const;
	Vector3d operator* (float arg) const;
	Vector3d operator-() const;
};

