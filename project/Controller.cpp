#include "Controller.h"
#include "qdebug.h"
#include "qdatetime.h"

void Controller::init(QLabel *canvas)
{
	_timer = new QTimer(nullptr);
	_timer->setSingleShot(true);
	connect(_timer, &QTimer::timeout, this, &Controller::tick);

	_renderThread = nullptr;
	_isRenderFinished.store(false);

	int w = canvas->width();
	int h = canvas->height();
	_image = new QImage(w, h, QImage::Format_RGB32);
	_scene = new Scene();
	//_scene->camera() = Camera(w, h, w / 2, Transform());

	_viewport = new Viewport(canvas, _image);
	_renderer = new Renderer(_scene, _image);

	_animator = new Animator();
	_playbackSpeed = 1;
	_cameraChanged = false;
}

Controller::~Controller()
{
	if (_renderThread != nullptr)
	{
		_renderThread->join();
		delete _renderThread;
	}

	delete _timer;
	delete _image;
	delete _scene;
	delete _viewport;
	delete _renderer;
	delete _animator;
}

int Controller::load(QString path)
{
	Loader loader(path);

	if (loader.success() == false)
		return -1;

	_timer->stop();
	stopRendThread();

	_animator->clear();
	_scene->clear();

	_animator->move(loader.getAnimator());
	_scene->move(loader.getScene());

	_animator->reset();
	_timer->start(5);

	return 0;
}

void Controller::start()
{
	_timer->start(1);

	//qDebug() << QTime().currentTime();

	//Every Line
	//QTime("11:54:48.783")
	//QTime("11:55:04.924")
	//qDebug() << QTime(11, 55, 04, 924).msecsTo(QTime(11, 54, 48, 783));

	// Bath
	//QTime("12:18:07.928")
	//QTime("12:18:19.704")
	//qDebug() << QTime(12, 18, 19, 704).msecsTo(QTime(12, 18, 07, 928));
	//qDebug() << QTime(13, 12, 39, 265).msecsTo(QTime(13, 12, 25, 700));
	//qDebug() << QTime(13, 14, 27, 89).msecsTo(QTime(13, 14, 13, 807));
	//qDebug() << QTime(13, 16, 16, 818).msecsTo(QTime(13, 16, 03, 875)); --- Best Result

	// Without sinc
	//QTime("12:45:31.587")
	//QTime("12:45:17.505")
	//qDebug() << QTime(12, 45, 17, 505).msecsTo(QTime(12, 45, 31, 587));
	//qDebug() << QTime(13, 02, 25, 625).msecsTo(QTime(13, 02, 11, 458));
}

void Controller::stop()
{
	_timer->stop();
}

void Controller::restart()
{
	stopRendThread();

	_animator->reset();
	_timer->start();
}

void Controller::setPlaybackSpeed(int speed)
{
	_playbackSpeed = speed;
}

void Controller::setCamera(const Camera &c)
{
	_scene->camera() = c;
	_cameraChanged = true;
}

void Controller::stopRendThread()
{
	if (_renderThread != nullptr)
	{
		_renderThread->join();
		delete _renderThread;
		_renderThread = nullptr;
		_isRenderFinished.exchange(false);
	}
}

void Controller::tick()
{
	if (_isRenderFinished.load())
	{
		stopRendThread();
		_viewport->show();
	}
	else if (_renderThread == nullptr)
	{
		if (_animator->finished() == false || _cameraChanged)
			_renderThread = new std::thread(&Controller::rend, this);
	}

	_timer->start(1);
}

void Controller::rend()
{
	_animator->update(_playbackSpeed);
	_renderer->render();
	_isRenderFinished.exchange(true);
}
