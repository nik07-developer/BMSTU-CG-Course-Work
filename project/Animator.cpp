#include "Animator.h"

#include <stdlib.h>
#include <cmath>

// KEY_FRAME

KeyFrame::KeyFrame() :
	ikey(0),
	value()
{
}

KeyFrame::KeyFrame(int i, Transform t) :
	ikey(i),
	value(t)
{
}

// ANIMATION

Animation::Animation() :
	_keys(QVector<KeyFrame>()),
	_prevFrame(nullptr),
	_nextFrame(nullptr)
{
}

void Animation::reset()
{
	_prevFrame = _keys.begin();
	_nextFrame = _prevFrame + 1;
}

void Animation::add(const KeyFrame &value)
{
	int indx = 0;
	while (indx < _keys.count() && _keys.at(indx).ikey < value.ikey)
		indx++;
	_keys.insert(indx, value);

	_prevFrame = _keys.begin();
	_nextFrame = _prevFrame + 1;
}

void Animation::translate(Transform *t, int frame)
{
	while (frame > _nextFrame->ikey)
	{
		_prevFrame++;
		_nextFrame++;
	}

	if (frame < _keys.last().ikey)
	{
		int a = _prevFrame->ikey;
		int b = _nextFrame->ikey;

		float k = ((float) frame - a) / (b - a);

		t->position = _prevFrame->value.position * (1.0f - k) + _nextFrame->value.position * k;
		t->rotation = _prevFrame->value.rotation * (1.0f - k) + _nextFrame->value.rotation * k;
	}
	else
	{
		*t = _keys.last().value;
	}
}

int Animation::length()
{
	return _keys.last().ikey;
}

// ANIMATOR

void Animator::move(Animator &src)
{
	for (int i = 0; i < src._animations.count(); i++)
		add(src._objects[i], src._animations[i]);

	src.clear();
}

void Animator::add(Transform *t, Animation a)
{
	_objects.append(t);
	_animations.append(a);
	_lastframe = std::max(_lastframe, a.length());
}

void Animator::update(int dframe)
{
	if (_curframe <= _lastframe)
		_curframe += dframe;

	for (int i = 0; i < _objects.count(); i++)
	{
		_animations[i].translate(_objects[i], _curframe);
	}
}

void Animator::reset()
{
	_curframe = -1;
	for (auto &a : _animations)
		a.reset();
}

void Animator::clear()
{
	_curframe = 0;
	_lastframe = 0;
	_animations.clear();
	_objects.clear();
}

bool Animator::finished() const
{
	return _curframe > _lastframe;
}
