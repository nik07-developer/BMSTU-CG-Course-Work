#include "Camera.h"

Camera::Camera(int w, int h, int d, Transform t, float size) :
	_width(w),
	_height(h),
	_distanse(d),
	_size(size),
	_resolution(float(w) / h)
{
	_transform = t;
}

Ray3d Camera::ray(int dx, int dy)
{
	Vector3d p(0, 0, -_distanse);
	//Vector3d s(-_width / 2 + dx, _height / 2 - dy, 0);

	Vector3d s(-_size / 2 + _size / _width * dx, _size / _resolution / 2 - _size / _resolution / _height * dy, 0);

	Vector3d direct(s - p);
	direct.normalize();
	_transform.directionToGlobal(direct);
	_transform.pointToGlobal(s);

	return Ray3d(s, direct);
}