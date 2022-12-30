#pragma once
#include "SceneObject.h"

class Robot : public Model
{
public:
	Robot(Transform t);
	virtual Hit3d hit(Ray3d ray) const override;

	virtual void Robot::paint(QColor clr) override;

	Transform *head() { return _head.transformP(); }
	Transform *body() { return _body.transformP(); }
	Transform *lhand() { return _lhand.transformP(); }
	Transform *rhand() { return _rhand.transformP(); }

private:
	Box _head;
	Cylinder _body;
	CylinderB _lhand;
	CylinderB _rhand;
};

