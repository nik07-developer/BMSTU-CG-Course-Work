#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include "qimage.h"
#include "qpushbutton.h"
#include "qslider.h"
#include "qfiledialog.h"
#include "qmessagebox.h"

#include "qthread.h"

#include "Controller.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() {};

private:
    Ui::MainWindowClass ui;
    Controller _controller;
    bool _pause;

private slots:
    void setCamera();
    void pause();
    void load();
};
