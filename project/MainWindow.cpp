#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	_pause(false)
{
	ui.setupUi(this);
	_controller.init(ui.canvas);

	connect(ui.btnStart, &QPushButton::clicked, this, &MainWindow::load);
	connect(ui.btnStop, &QPushButton::clicked, this, &MainWindow::pause);
	connect(ui.btnRestart, &QPushButton::clicked, &_controller, &Controller::restart);

	connect(ui.speed, &QSlider::valueChanged, &_controller, &Controller::setPlaybackSpeed);

	//connect(ui.cx, &QDoubleSpinBox::valueChanged, this, &MainWindow::setCamera);
	//connect(ui.cy, &QDoubleSpinBox::valueChanged, this, &MainWindow::setCamera);
	//connect(ui.cz, &QDoubleSpinBox::valueChanged, this, &MainWindow::setCamera);
	//connect(ui.cdx, &QDoubleSpinBox::valueChanged, this, &MainWindow::setCamera);
	//connect(ui.cdy, &QDoubleSpinBox::valueChanged, this, &MainWindow::setCamera);
	//connect(ui.cdz, &QDoubleSpinBox::valueChanged, this, &MainWindow::setCamera);
	connect(ui.btnSetCamera, &QPushButton::clicked, this, &MainWindow::setCamera);

}

void MainWindow::load()
{
	auto filename = QFileDialog::getOpenFileName(this, tr("Open Scene"), ".", tr("Files (*.txt)"));
	if (_controller.load(filename) == 0)
	{
		_controller.start();
	}
	else
	{
		QMessageBox::warning(this, QString("Error"), "Error while loading scene");
	}
}

void MainWindow::pause()
{
	_pause = !_pause;

	if (_pause)
		_controller.stop();
	else
		_controller.start();
}

void MainWindow::setCamera()
{
	Transform t(Vector3d(ui.cx->value(), ui.cy->value(), ui.cz->value()),
				Vector3d(ui.cdx->value(), ui.cdy->value(), ui.cdz->value()));
	Camera c(ui.canvas->width(), ui.canvas->height(), ui.canvas->width() / 2, t);
	_controller.setCamera(c);
}
