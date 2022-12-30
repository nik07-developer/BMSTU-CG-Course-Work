#include "Vector3d.h"

Vector3d::Vector3d() :
	x(0),
	y(0),
	z(0)
{
}

Vector3d::Vector3d(float x, float y, float z) :
	x(x),
	y(y),
	z(z)
{
}

Vector3d::Vector3d(const Vector3d &base)
{
	x = base.x;
	y = base.y;
	z = base.z;
}

float Vector3d::length() const
{
	return sqrt(x * x + y * y + z * z);
}

void Vector3d::normalize()
{
	float len = length();
	x /= len;
	y /= len;
	z /= len;
}

Vector3d Vector3d::normalized() const
{
	float len = sqrt(x * x + y * y + z * z);
	return Vector3d(x / len, y / len, z / len);
}

void Vector3d::rotateX(float rx)
{
	rx = rx * M_PI / 180.0;
	float ny = y * cos(rx) - z * sin(rx);
	float nz = y * sin(rx) + z * cos(rx);
	y = ny;
	z = nz;
}

void Vector3d::rotateY(float ry)
{
	ry = ry * M_PI / 180.0;
	float nx = x * cos(ry) + z * sin(ry);
	float nz = -x * sin(ry) + z * cos(ry);
	x = nx;
	z = nz;
}

void Vector3d::rotateZ(float rz)
{
	rz = rz * M_PI / 180.0;
	float nx = x * cos(rz) + y * sin(rz);
	float ny = -x * sin(rz) + y * cos(rz);
	x = nx;
	y = ny;
}

void Vector3d::rotate(const Vector3d &rotor)
{
	rotateX(rotor.x);
	rotateY(rotor.y);
	rotateZ(rotor.z);
}

void Vector3d::rotateBack(const Vector3d &rotor)
{
	rotateZ(-rotor.z);
	rotateY(-rotor.y);
	rotateX(-rotor.x);
}

void Vector3d::move(const Vector3d &offset)
{
	x += offset.x;
	y += offset.y;
	z += offset.z;
}

float Vector3d::dot(const Vector3d &arg) const
{
	return x * arg.x + y * arg.y + z * arg.z;
}

Vector3d Vector3d::operator+(const Vector3d &a) const
{
	return Vector3d(x + a.x, y + a.y, z + a.z);
}

Vector3d Vector3d::operator-(const Vector3d &a) const
{
	return Vector3d(x - a.x, y - a.y, z - a.z);
}

Vector3d Vector3d::operator*(float a) const
{
	return Vector3d(x * a, y * a, z * a);
}

Vector3d Vector3d::operator-() const
{
	return Vector3d(-x, -y, -z);
}
