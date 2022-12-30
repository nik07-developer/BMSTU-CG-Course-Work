#pragma once

#include "qstring.h"
#include "qfile.h"
#include "qmap.h"

#include "Scene.h"
#include "Animator.h"
#include "Robot.h"
#include "Human.h"

class Loader
{
public:
	Loader(QString path);
	bool success();

	Scene& getScene();
	Animator& getAnimator();

private:
	int _exitCode;
	Scene _scene;
	Animator _animator;
};

