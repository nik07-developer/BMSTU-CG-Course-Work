#pragma once

#include "memory"

#include "qimage.h"
#include "qpixmap.h"
#include "qlabel.h"

class Viewport
{
public:
	Viewport(QLabel *canvas, QImage *image);
	~Viewport() {};

	void show();

	QImage *_image;
	QLabel *_canvas;
};

