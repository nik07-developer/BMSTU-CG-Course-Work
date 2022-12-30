#include "Renderer.h"

Renderer::Renderer(Scene *scene, QImage *image) :
	_scene(scene),
	_image(image),
	_background(qRgb(200, 200, 200)),
	mt()
{
	_threadCount = std::thread::hardware_concurrency() - 1;
	if (_threadCount < 1)
		_threadCount = 4;

	//_bathSize = image->height() / _threadCount;
	_bathSize = 40;

	_threads = nullptr;
	_lineCounter.store(0);
}

Renderer::~Renderer()
{
	if (_threads != nullptr)
	{
		for (int i = 0; i < _threadCount; i++)
			_threads[i].join();

		delete[] _threads;
		_threads = nullptr;
	}
}

void Renderer::render()
{
	_threads = new std::thread[_threadCount];
	_lineCounter.store(0);

	for (int i = 0; i < _threadCount; i++)
	{
		int lines = _lineCounter.load();
		_threads[i] = std::thread(&Renderer::renderBath, this, lines);
		_lineCounter.fetch_add(_bathSize);
	}

	for (int i = 0; i < _threadCount; i++)
		_threads[i].join();

	delete[] _threads;
	_threads = nullptr;
}

bool Renderer::raycast(Ray3d ray, Hit3d &out) const
{
	Hit3d hit;
	if (ray.direction.length() <= 0)
		return false;

	for (auto it = _scene->beginModels(); it < _scene->endModels(); it++)
	{
		Hit3d tmp = (*it)->hit(ray);
		if (tmp.distance > 0 && (tmp.distance < hit.distance || hit.distance <= 0))
			hit = tmp;
	}

	if (hit.distance > 0)
		out = hit;

	return hit.distance > 0;
}

static QColor blend(QColor x, QColor y, float i)
{
	int r = fmin(255, x.red() * (1 - i) + y.red() * i);
	int g = fmin(255, x.green() * (1 - i) + y.green() * i);
	int b = fmin(255, x.blue() * (1 - i) + y.blue() * i);

	return QColor(r, g, b);
}

QColor Renderer::calc_color(Hit3d hit, Vector3d viewDirection) const
{
	QColor tmp(hit.color);
	float intense = 0;

	for (auto it = _scene->beginLights(); it < _scene->endLights(); it++)
	{
		Ray3d ray(hit.point + (*it)->direction(hit.point), (*it)->direction(hit.point));
		Hit3d bhit;
		if (raycast(ray, bhit) == false)
		{
			intense += (*it)->intensity(hit, viewDirection);
		}
	}

	QColor result;

	if (intense < 1.0)
		result = blend(QColor(0, 0, 0), hit.color, intense);
	else
		result = blend(hit.color, QColor(255, 255, 255), intense - 1.0);

	return result;
}

void Renderer::renderBath(int startY)
{
	Camera cam = _scene->camera();
	int w = _image->width();
	int h = _image->height();

	while (startY < h)
	{
		int dy = 0;
		while (dy < _bathSize && startY + dy < h)
		{
			for (int dx = 0; dx < w; dx++)
			{
				Ray3d ray(cam.ray(dx, startY + dy));
				Hit3d hit;

				if (raycast(ray, hit))
				{
					auto clr = calc_color(hit, ray.direction).rgba();
					mt.lock();
					_image->setPixel(dx, startY + dy, clr);
					mt.unlock();
				}
				else
				{
					auto clr = _background.rgba();
					mt.lock();
					_image->setPixel(dx, startY + dy, clr);
					mt.unlock();
				}
			}

			dy++;
		}

		startY = _lineCounter.load();
		_lineCounter.fetch_add(_bathSize);
	}
}

/*
void Renderer::renderBath(int startY)
{
	Camera cam = _scene->camera();
	int w = _image->width();
	int h = _image->height();

	for (int dy = startY; dy < startY + _bathSize; dy++)
	{
		//QRgb *line = reinterpret_cast<QRgb *>(_image->scanLine(dy));

		for (int dx = 0; dx < w; dx++)
		{
			Ray3d ray(cam.ray(dx, dy));
			Hit3d hit;

			if (raycast(ray, hit))
			{
				auto clr = calc_color(hit, ray.direction);
				mt.lock();
				_image->setPixel(dx, dy, clr.rgb());
				//line[dx] = clr.rgba();
				mt.unlock();
			}
			else
			{
				mt.lock();
				_image->setPixel(dx, dy, _background.rgb());
				//line[dx] = _background.rgba();
				mt.unlock();
			}

		}
	}
}
*/
