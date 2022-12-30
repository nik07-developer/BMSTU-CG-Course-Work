#include "Ray3d.h"

Ray3d::Ray3d(Vector3d point, Vector3d direction) :
	point(point),
	direction(direction)
{
}

Hit3d::Hit3d() :
	distance(-1.0),
	point(Vector3d::zero()),
	normal(Vector3d::zero()),
	color(qRgb(0,0,0))
{
}

Hit3d::Hit3d(float d, Vector3d p, Vector3d n, QColor c) :
	distance(d),
	point(p),
	normal(n),
	color(c)
{
}
