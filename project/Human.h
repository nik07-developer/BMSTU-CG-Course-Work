#pragma once
#include "SceneObject.h"

class Human : public Model
{
public:
	Human(Transform t);
	virtual Hit3d hit(Ray3d ray) const override;
	virtual void paint(QColor clr) override;

	Transform *head() { return _head.transformP(); }
	Transform *body() { return _body.transformP(); }
	Transform *lhand() { return _lhand.transformP(); }
	Transform *rhand() { return _rhand.transformP(); }

private:
	Sphere _head;
	Cylinder _body;
	CylinderB _lhand;
	CylinderB _rhand;
};

