#pragma once

#include "Transform.h"
#include <qvector.h>

struct KeyFrame
{
	int ikey;
	Transform value;

	KeyFrame();
	KeyFrame(int i, Transform t);
};

class Animation
{
public:
	Animation();

	void reset();
	void add(const KeyFrame &value);
	void translate(Transform *t, int frame);
	int length();

private:
	QVector<KeyFrame> _keys;
	QVector<KeyFrame>::iterator _prevFrame;
	QVector<KeyFrame>::iterator _nextFrame;
};

class Animator
{
public:
	void move(Animator &src);
	void add(Transform *t, Animation a);
	void update(int dframe);
	void reset();
	void clear();

	bool finished() const;

private:
	int _curframe = 0;
	int _lastframe = 0;
	QVector<Animation> _animations;
	QVector<Transform *> _objects;
};

