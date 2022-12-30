#pragma once

#include "SceneObject.h"
#include "Camera.h"

#include "qvector.h"

class Scene
{
public:
	Scene();
	~Scene();

	void clear();

	void move(Scene &src);

	void add(Model *md) { _models.append(md); }
	void add(Light *lg) { _lights.append(lg); }

	Camera &camera() { return _camera; }
	QVector<Model *>::iterator beginModels() { return _models.begin(); }
	QVector<Model *>::iterator endModels() { return _models.end(); }

	QVector<Light *>::iterator beginLights() { return _lights.begin(); }
	QVector<Light *>::iterator endLights() { return _lights.end(); }

private:
	Camera _camera;
	QVector<Model *> _models;
	QVector<Light *> _lights;
};

