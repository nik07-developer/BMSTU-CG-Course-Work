#include "Viewport.h"

Viewport::Viewport(QLabel *canvas, QImage *image) :
	_canvas(canvas),
	_image(image)
{
}

void Viewport::show()
{
	auto map = QPixmap::fromImage(*_image);
	_canvas->setPixmap(map);
}