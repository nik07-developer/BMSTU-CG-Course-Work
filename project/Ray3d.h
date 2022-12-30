#pragma once

#include "qcolor.h"

#include "Vector3d.h"

struct Ray3d
{
	Vector3d point;
	Vector3d direction;

	Ray3d(Vector3d point, Vector3d direction);
};

struct Hit3d
{
	float distance;
	Vector3d point;
	Vector3d normal;
	QColor color;

	Hit3d();
	Hit3d(float d, Vector3d p, Vector3d n, QColor c);
	inline bool is() { return distance > 0; }
};

