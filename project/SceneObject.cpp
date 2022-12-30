#include "SceneObject.h"

Transform &SceneObject::transform()
{
	return _transform;
}

Transform *SceneObject::transformP()
{
	return &_transform;
}

Sphere::Sphere(Vector3d point, float radius) :
	_radius(radius)
{
	_transform = Transform(point, Vector3d::zero());
	_color = QColor(255, 0, 0);
}

Hit3d Sphere::hit(Ray3d ray) const
{
	Hit3d hit;

	float x0 = ray.point.x - _transform.position.x;
	float y0 = ray.point.y - _transform.position.y;
	float z0 = ray.point.z - _transform.position.z;

	float l = ray.direction.x;
	float m = ray.direction.y;
	float n = ray.direction.z;

	float a = (l * l + m * m + n * n);
	float k = (l * x0 + m * y0 + n * z0);
	float c = (x0 * x0 + y0 * y0 + z0 * z0 - _radius * _radius);

	float D1 = k * k - a * c;

	if (D1 < 0)
	{
		hit.distance = -1.0;
	}
	else
	{
		float t1 = (-k - sqrt(D1)) / a;
		float t2 = (-k + sqrt(D1)) / a;

		float t = fmin(t1, t2);
		if (t < 0)
			t = fmax(t1, t2);

		hit.distance = t;
		hit.point = ray.point + ray.direction * t;
		hit.normal = (hit.point - _transform.position).normalized();
		hit.color = _color;
	}

	return hit;
}

Cylinder::Cylinder(Transform t, float radius, float height) :
	_radius(radius),
	_height(height)
{
	_transform = t;
	_color = QColor(255, 0, 0);
}

Hit3d Cylinder::hit(Ray3d ray) const
{
	Hit3d hit;
	hit.distance = -1.0f;

	_transform.rayToLocal(ray);

	float x0 = ray.point.x;
	float y0 = ray.point.y;
	float z0 = ray.point.z;

	float l = ray.direction.x;
	float m = ray.direction.y;
	float n = ray.direction.z;

	if (fabs(n) > 1e-12)
	{
		float z = _height / 2;
		float t = (z - z0) / n;
		float x = l * t + x0;
		float y = m * t + y0;

		if (t > 0 && x * x + y * y <= _radius * _radius)
		{
			hit.distance = t;
			hit.point = Vector3d(x, y, z);
			hit.normal = Vector3d::forward();
		}

		z = -z;
		t = (z - z0) / n;
		x = l * t + x0;
		y = m * t + y0;

		if (t > 0 && x * x + y * y <= _radius * _radius)
		{
			if ((hit.distance > 0 && t < hit.distance) || hit.distance < 0)
			{
				hit.distance = t;
				hit.point = Vector3d(x, y, z);
				hit.normal = Vector3d::back();
			}
		}
	}

	float a = (l * l + m * m);
	float k = (l * x0 + m * y0);
	float c = (x0 * x0 + y0 * y0 - _radius * _radius);

	float D1 = k * k - a * c;

	if (D1 >= 0)
	{
		float t1 = (-k - sqrt(D1)) / a;
		float t2 = (-k + sqrt(D1)) / a;

		float t = fmin(t1, t2);
		if (t < 0)
			t = fmax(t1, t2);

		if (t > 0 && fabs(n * t + z0) < _height / 2)
		{
			if ((hit.distance > 0 && t < hit.distance) || hit.distance < 0)
			{
				hit.distance = t;
				hit.point = ray.point + ray.direction * t;
				hit.normal = Vector3d(l * t + x0, m * t + y0, 0).normalized();
			}
		}
	}

	if (hit.distance > 0)
	{
		_transform.hitToGlobal(hit);
		hit.color = _color;
	}

	return hit;
}

Box::Box(Transform t, float x, float y, float z)
{
	_b1[0] = -x / 2;
	_b1[1] = -y / 2;
	_b1[2] = -z / 2;

	_b2[0] = x / 2;
	_b2[1] = y / 2;
	_b2[2] = z / 2;

	_transform = t;
	_color = QColor(255, 0, 0);
}

Hit3d Box::hit(Ray3d ray) const
{
	Hit3d hit;
	hit.distance = -1.0f;

	_transform.rayToLocal(ray);

	float p[3] = { ray.point.x, ray.point.y, ray.point.z };
	float d[3] = { ray.direction.x, ray.direction.y, ray.direction.z };

	float tn = std::numeric_limits<float>::min();
	float tf = std::numeric_limits<float>::max();

	for (int i = 0; i < 3; i++)
		if (fabs(d[i]) < 1e-12)
		{
			if (p[i] < _b1[i] || p[i] > _b2[i])
				return hit;
		}
		else
		{
			float t1 = (_b1[i] - p[i]) / d[i];
			float t2 = (_b2[i] - p[i]) / d[i];

			if (t1 > t2)
				std::swap(t1, t2);

			tn = fmax(tn, t1);
			tf = fmin(tf, t2);

			if (tn > tf || tf < 0)
				return hit;
		}

	if (0 < tn && tn < tf)
	{
		hit.distance = tn;
		hit.point = ray.point + ray.direction * tn;

		if (fabs(hit.point.x - _b1[0]) < 1e-3) hit.normal = Vector3d::left();
		else if (fabs(hit.point.x - _b2[0]) < 1e-3) hit.normal = Vector3d::right();
		else if (fabs(hit.point.y - _b1[1]) < 1e-3) hit.normal = Vector3d::down();
		else if (fabs(hit.point.y - _b2[1]) < 1e-3) hit.normal = Vector3d::up();
		else if (fabs(hit.point.z - _b1[2]) < 1e-3) hit.normal = Vector3d::back();
		else if (fabs(hit.point.z - _b2[2]) < 1e-3) hit.normal = Vector3d::forward();

		_transform.hitToGlobal(hit);
		hit.color = _color;
	}

	return hit;
}


AmbientLight::AmbientLight(float intensity) :
	_intensity(intensity)
{
}

Vector3d AmbientLight::direction(Vector3d point) const
{
	return Vector3d::zero();
}

float AmbientLight::intensity(Hit3d hit, Vector3d view) const
{
	return _intensity;
}

DirectionalLight::DirectionalLight(Vector3d direction, float intensity) :
	_direction(direction.normalized()),
	_intensity(intensity)
{
}

Vector3d DirectionalLight::direction(Vector3d point) const
{
	return Vector3d(-_direction);
}

float DirectionalLight::intensity(Hit3d hit, Vector3d view) const
{
	Vector3d reflected = _direction + hit.normal * 2.0 * hit.normal.dot(-_direction);
	float cs1 = (-view).dot(reflected);
	float cs2 = (hit.normal).dot(-_direction);

	return 0.5f * _intensity * pow(cs1, 50) + 0.5f * _intensity * cs2;
}

CylinderB::CylinderB(Transform t, float radius, float height) :
	_radius(radius),
	_height(height)
{
	_transform = t;
	_color = QColor(255, 0, 0);
}

Hit3d CylinderB::hit(Ray3d ray) const
{
	_transform.rayToLocal(ray);

	Transform t(Vector3d(0, _height / 2, 0), Vector3d(90, 0, 0));
	Cylinder tmp(t, _radius, _height);

	Hit3d hit = tmp.hit(ray);
	hit.color = _color;

	_transform.hitToGlobal(hit);

	return hit;
}

void Model::paint(QColor clr)
{
	_color = clr;
}
