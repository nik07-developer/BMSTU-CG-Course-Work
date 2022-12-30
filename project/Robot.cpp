#include "Robot.h"

Robot::Robot(Transform t) :
	_head(Transform(), 150, 100, 100),
	_body(Transform(), 50, 200),
	_rhand(Transform(), 25, 100),
	_lhand(Transform(), 25, 100)
{
	_transform = t;
	_color = QColor(255, 0, 0);
}

void Robot::paint(QColor clr)
{
	_head.paint(clr);
	_body.paint(clr);
	_lhand.paint(clr);
	_rhand.paint(clr);
}

Hit3d Robot::hit(Ray3d ray) const
{
	_transform.pointToLocal(ray.point);
	_transform.directionToLocal(ray.direction);

	Hit3d hit, tmp;
	hit.distance = -1.0f;

	{
		Ray3d r(ray);
		Transform t(Vector3d(0, 150, 0), Vector3d(0, 0, 0));
		t.rayToLocal(r);
		tmp = _head.hit(r);
		if (tmp.distance > 0 && (tmp.distance < hit.distance || hit.distance <= 0))
		{
			hit = tmp;
			t.hitToGlobal(hit);
		}
	}

	{
		Ray3d r(ray);
		Transform t(Vector3d(), Vector3d(90, 0, 0));
		t.rayToLocal(r);
		tmp = _body.hit(r);
		if (tmp.distance > 0 && (tmp.distance < hit.distance || hit.distance <= 0))
		{
			hit = tmp;
			t.hitToGlobal(hit);
		}
	}

	{
		Ray3d r(ray);
		Transform t(Vector3d(-75, 90, 0), Vector3d(0, 0, -180));
		t.rayToLocal(r);
		tmp = _lhand.hit(r);
		if (tmp.distance > 0 && (tmp.distance < hit.distance || hit.distance <= 0))
		{
			hit = tmp;
			t.hitToGlobal(hit);
		}
	}

	{
		Ray3d r(ray);
		Transform t(Vector3d(75, 90, 0), Vector3d(0, 0, 180));
		t.rayToLocal(r);
		tmp = _rhand.hit(r);
		if (tmp.distance > 0 && (tmp.distance < hit.distance || hit.distance <= 0))
		{
			hit = tmp;
			t.hitToGlobal(hit);
		}
	}

	if (hit.distance > 0)
		_transform.hitToGlobal(hit);
	
	return hit;
}
