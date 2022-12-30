#pragma once

#include "Vector3d.h"
#include "Ray3d.h"

struct Transform
{
	Vector3d position;
	Vector3d rotation;

	Transform();
	Transform(const Transform &base);
	Transform(const Vector3d &pos, const Vector3d &rt);

	void move(const Vector3d &offset);
	void rotate(const Vector3d &xyz);

	void rayToLocal(Ray3d &ray) const;
	void hitToGlobal(Hit3d &hit) const;

	void pointToLocal(Vector3d &point) const;
	void pointToGlobal(Vector3d &point) const;

	void directionToLocal(Vector3d &direction) const;
	void directionToGlobal(Vector3d &direction) const;
};

