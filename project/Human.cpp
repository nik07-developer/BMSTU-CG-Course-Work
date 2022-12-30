#include "Human.h"

Human::Human(Transform t) :
	_body(Transform(), 50, 200),
	_head(Vector3d(), 60),
	_rhand(Transform(), 25, 100),
	_lhand(Transform(), 25, 100)
{
	_transform = t;
}

void Human::paint(QColor clr)
{
	_head.paint(clr);
	_body.paint(clr);
	_lhand.paint(clr);
	_rhand.paint(clr);
}

Hit3d Human::hit(Ray3d ray) const
{
	_transform.pointToLocal(ray.point);
	_transform.directionToLocal(ray.direction);

	Hit3d hit, tmp;
	hit.distance = -1.0f;

	{
		Ray3d r(ray);
		Transform t(Vector3d(0, 140, 0), Vector3d(0, 0, 0));
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

