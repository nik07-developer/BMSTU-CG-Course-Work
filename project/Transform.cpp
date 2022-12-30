#include "Transform.h"
#include <cmath>

Transform::Transform() :
	position(Vector3d::zero()),
	rotation(Vector3d::zero())
{
}

Transform::Transform(const Transform &base) :
	position(base.position),
	rotation(base.rotation)
{
}

Transform::Transform(const Vector3d &pos, const Vector3d &rot) :
	position(pos),
	rotation(rot)
{
}

void Transform::move(const Vector3d &offset)
{
	position = position + offset;
}

void Transform::rotate(const Vector3d &xyz)
{
	rotation = rotation + xyz;
}


void Transform::rayToLocal(Ray3d &ray) const
{
	pointToLocal(ray.point);
	directionToLocal(ray.direction);
}

void Transform::hitToGlobal(Hit3d &hit) const
{
	pointToGlobal(hit.point);
	directionToGlobal(hit.normal);
}

void Transform::pointToLocal(Vector3d &vec) const
{
	vec.move(-position);
	vec.rotateBack(rotation);
}

void Transform::pointToGlobal(Vector3d &vec) const
{
	vec.rotate(rotation);
	vec.move(position);
}

void Transform::directionToLocal(Vector3d &direction) const
{
	direction.rotateBack(rotation);
}

void Transform::directionToGlobal(Vector3d &direction) const
{
	direction.rotate(rotation);
}
