#pragma once

#include "Vector3d.h"
#include "SceneObject.h"

class Camera : public SceneObject
{
public:
	Camera(int w, int h, int d, Transform t, float size = 800.0f);
	Ray3d ray(int dx, int dy);

private:
	int _width;
	int _height;
	int _distanse;

	float _size;
	float _resolution;
};

