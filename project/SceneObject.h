#pragma once

#include "Transform.h"
#include "Ray3d.h"

class SceneObject
{
public:
	Transform &transform();
	Transform *transformP();

protected:
	Transform _transform;
};

class Model : public SceneObject
{
public:
	virtual Hit3d hit(Ray3d) const = 0;
	virtual void paint(QColor clr);

protected:
	QColor _color;
};

class Light : public SceneObject
{
public:
	virtual Vector3d direction(Vector3d point) const = 0;
	//virtual Vector3d reflected(Hit3d hit) const = 0;
	//virtual float intensity(Vector3d point) const = 0;
	virtual float intensity(Hit3d hit, Vector3d view) const = 0;
};

class Sphere : public Model
{
public:
	Sphere(Vector3d point, float radius);
	virtual Hit3d hit(Ray3d ray) const override;

private:
	float _radius;
};

class Cylinder : public Model
{
public:
	Cylinder(Transform t, float radius, float height);
	virtual Hit3d hit(Ray3d ray) const override;

private:
	float _radius;
	float _height;
};

class CylinderB : public Model
{
public:
	CylinderB(Transform t, float radius, float height);
	virtual Hit3d hit(Ray3d ray) const override;

private:
	float _radius;
	float _height;
};

class Box : public Model
{
public:
	Box(Transform t, float x, float y, float z);
	virtual Hit3d hit(Ray3d ray) const override;

private:
	float _b1[3];
	float _b2[3];
};

class AmbientLight : public Light
{
public:
	AmbientLight(float intensity);
	virtual Vector3d direction(Vector3d point) const override;
	//virtual Vector3d reflected(Hit3d hit) const override;
	//virtual float intensity(Vector3d point) const override;
	virtual float intensity(Hit3d hit, Vector3d view) const override;

private:
	float _intensity;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight(Vector3d direction, float intensity);
	virtual Vector3d direction(Vector3d point) const override;
	//virtual Vector3d reflected(Hit3d hit) const override;
	virtual float intensity(Hit3d hit, Vector3d view) const override;

private:
	Vector3d _direction;
	float _intensity;
};