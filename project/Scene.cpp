#include "Scene.h"

Scene::Scene() :
	_camera(Camera(400, 300, 200, Transform())),
	_models(),
	_lights()
{
}

Scene::~Scene()
{
	for (int i = 0; i < _models.count(); i++)
		delete _models[i];

	for (int i = 0; i < _lights.count(); i++)
		delete _lights[i];
}

void Scene::clear()
{
	for (int i = 0; i < _models.count(); i++)
		delete _models[i];

	for (int i = 0; i < _lights.count(); i++)
		delete _lights[i];

	_models.clear();
	_lights.clear();
}

void Scene::move(Scene &src)
{
	_camera = src._camera;

	for (int i = 0; i < src._models.count(); i++)
		_models.append(src._models[i]);

	for (int i = 0; i < src._lights.count(); i++)
		_lights.append(src._lights[i]);

	src._models.clear();
	src._lights.clear();
}
