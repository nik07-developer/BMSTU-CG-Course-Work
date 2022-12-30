#pragma once

#include "memory"
#include <cmath>

#include "qimage.h"
#include "thread"
#include <mutex>

#include "Scene.h"
#include "Camera.h"


class Renderer
{
public:
	Renderer(Scene *scene, QImage *image);
	~Renderer();

	void render();

private:
	Scene *_scene;
	QImage *_image;
	std::atomic_int _lineCounter;
	int _threadCount;
	int _bathSize;
	QColor _background;

	std::mutex mt;

	std::thread *_threads;
	void renderBath(int startY);
	bool raycast(Ray3d ray, Hit3d &out) const;
	QColor calc_color(Hit3d hit, Vector3d viewDirection) const;
};

