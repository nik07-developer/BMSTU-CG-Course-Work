#pragma once

#include "memory"

#include "qmainwindow.h"
#include "qobject.h"
#include "qlabel.h"
#include "qimage.h"
#include "qpixmap.h"
#include "qtimer.h"

#include "Viewport.h"
#include "Renderer.h"
#include "Animator.h"

#include "Loader.h"
#include "SceneObject.h"
#include "Robot.h"

class Controller : public QObject
{
	Q_OBJECT

public:
	~Controller();
	void init(QLabel *canvas);
	int load(QString path);

public slots:
	void start();
	void stop();
	void restart();
	
	void setPlaybackSpeed(int speed);
	void setCamera(const Camera &c);

private slots:
	void tick();

private:
	QTimer *_timer;
	QImage *_image;
	Scene *_scene;
	Viewport *_viewport;
	Renderer *_renderer;
	Animator *_animator;

	std::thread *_renderThread;
	std::atomic_bool _isRenderFinished;
	void rend();
	void stopRendThread();

	int _playbackSpeed;
	bool _cameraChanged;
};

