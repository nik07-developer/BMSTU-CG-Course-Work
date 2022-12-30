#include "Loader.h"

static QMap<QString, Model *> models;
static QMap<QString, Transform *> transforms;
static QMap<QString, Animation> animations;

typedef int (*Func)(Scene &, Animator &, QStringList &);

int nope(Scene &scene, Animator &animator, QStringList &args)
{
	return 0;
}

int addCamera(Scene &scene, Animator &animator, QStringList &args)
{
	if (args.count() < 4)
		return -1;

	bool flag[] = { false, false, false };

	int w = args[1].toInt(&flag[0]);
	int h = args[2].toInt(&flag[1]);
	int d = w * args[3].toFloat(&flag[2]);

	if (flag[0] && flag[1] && flag[2] == false)
		return -1;

	scene.camera() = Camera(w, h, d, Transform());
	return 0;
}

int paint(Scene &scene, Animator &animator, QStringList &args)
{
	if (args.count() < 5 || !models.contains(args[1]))
		return -1;

	Transform tmp;

	int a[3];
	bool flag = true;
	for (int i = 0; i < 3; i++)
	{
		a[i] = args[i + 2].toInt(&flag);
		if (flag == false)
			return -1;
	}

	models[args[1]]->paint(QColor(a[0], a[1], a[2]));

	return 0;
}

int addBox(Scene &scene, Animator &animator, QStringList &args)
{
	if (args.count() < 11)
		return -1;

	Transform tmp;

	float a[9];
	bool flag = true;
	for (int i = 0; i < 9; i++)
	{
		a[i] = args[i + 2].toFloat(&flag);
		if (flag == false)
			return -1;
	}

	tmp = Transform(Vector3d(a[3], a[4], a[5]), Vector3d(a[6], a[7], a[8]));
	Box *bx = new Box(tmp, a[0], a[1], a[2]);

	models.insert(args[1], bx);
	transforms.insert(args[1], bx->transformP());

	return 0;
}

int addRobot(Scene &scene, Animator &animator, QStringList &args)
{
	if (args.count() < 2)
		return -1;

	Transform tmp;

	if (args.count() == 8)
	{
		float a[6];
		bool flag = true;
		for (int i = 0; i < 6; i++)
		{
			a[i] = args[i + 2].toFloat(&flag);
			if (flag == false)
				return -1;
		}

		tmp = Transform(Vector3d(a[0], a[1], a[2]), Vector3d(a[3], a[4], a[5]));
	}

	Robot *rb = new Robot(tmp);

	models.insert(args[1], rb);
	transforms.insert(args[1], rb->transformP());

	transforms.insert(args[1] + ".head", rb->head());
	transforms.insert(args[1] + ".body", rb->body());
	transforms.insert(args[1] + ".lhand", rb->lhand());
	transforms.insert(args[1] + ".rhand", rb->rhand());

	return 0;
}

int addHuman(Scene &scene, Animator &animator, QStringList &args)
{
	if (args.count() < 2)
		return -1;

	Transform tmp;

	if (args.count() == 8)
	{
		float a[6];
		bool flag = true;
		for (int i = 0; i < 6; i++)
		{
			a[i] = args[i + 2].toFloat(&flag);
			if (flag == false)
				return -1;
		}

		tmp = Transform(Vector3d(a[0], a[1], a[2]), Vector3d(a[3], a[4], a[5]));
	}

	Human *hm = new Human(tmp);

	models.insert(args[1], hm);
	transforms.insert(args[1], hm->transformP());

	transforms.insert(args[1] + ".head", hm->head());
	transforms.insert(args[1] + ".body", hm->body());
	transforms.insert(args[1] + ".lhand", hm->lhand());
	transforms.insert(args[1] + ".rhand", hm->rhand());

	return 0;
}

int addAmbientLight(Scene &scene, Animator &animator, QStringList &args)
{
	if (args.count() < 2)
		return -1;

	bool flag = false;
	float value = args[1].toFloat(&flag);
	if (flag == false)
		return -1;

	auto lg = new AmbientLight(value);
	scene.add(lg);

	return 0;
}

int addDirectionalLight(Scene &scene, Animator &animator, QStringList &args)
{
	if (args.count() < 5)
		return -1;

	float a[4];
	bool flag = false;
	for (int i = 0; i < 4; i++)
	{
		a[i] = args[i + 1].toFloat(&flag);
		if (flag == false)
			return -1;
	}

	auto lg = new DirectionalLight(Vector3d(a[1], a[2], a[3]), a[0]);
	scene.add(lg);

	return 0;
}

int addAnimation(Scene &scene, Animator &animator, QStringList &args)
{

	if (args.count() < 2)
		return -1;

	bool flagg = transforms.contains(args[1]);
	if (args.count() < 2 || !flagg)
		return -1;

	animations.insert(args[1], Animation());

	return 0;
}

int addKeyFrame(Scene &scene, Animator &animator, QStringList &args)
{
	if (args.count() < 3 || !transforms.contains(args[2]))
		return -1;

	Transform *obj = transforms[args[2]];
	Transform tmp = *obj;

	int ikey;
	bool flag = true;

	ikey = args[1].toInt(&flag);
	if (flag == false)
		return -1;

	if (args.count() == 9)
	{
		float a[6];
		for (int i = 0; i < 6; i++)
		{
			a[i] = args[i + 3].toFloat(&flag);
			if (flag == false)
				return -1;
		}

		tmp = Transform(Vector3d(a[0], a[1], a[2]), Vector3d(a[3], a[4], a[5]));
	}

	animations[args[2]].add(KeyFrame(ikey, tmp));

	return 0;
}

static QMap<QString, Func> cmd = {
	{"/", nope},
	{"/camera", addCamera},
	{"/amb_light", addAmbientLight},
	{"/dir_light", addDirectionalLight},
	{"/box", addBox},
	{"/robot", addRobot},
	{"/human", addHuman},
	{"/paint", paint},
	{"/anim", addAnimation},
	{"/key", addKeyFrame}
};

Loader::Loader(QString path) :
	_exitCode(0),
	_scene(),
	_animator()
{
	models.clear();
	transforms.clear();
	animations.clear();

	QFile file(path);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		while (!file.atEnd() && _exitCode == 0)
		{
			QString str = file.readLine().trimmed();
			QStringList lst = str.split(" ");

			if (str.length() > 1)
			{
				if (cmd[lst[0]] != nullptr)
					_exitCode = cmd[lst[0]](_scene, _animator, lst);
				else
					_exitCode = 1;
			}
		}
	}
	else
	{
		_exitCode = 1;
	}

	for (auto &a : models.keys())
		_scene.add(models[a]);

	for (auto &a : animations.keys())
		_animator.add(transforms[a], animations[a]);
}

bool Loader::success()
{
	return _exitCode == 0;
}

Scene &Loader::getScene()
{
	return _scene;
}

Animator &Loader::getAnimator()
{
	return _animator;
}
