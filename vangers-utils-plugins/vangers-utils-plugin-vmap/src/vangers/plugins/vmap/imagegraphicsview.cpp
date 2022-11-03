#include <vangers/plugins/image/graphicsviewzoom.h>
#include "imagegraphicsview.h"

#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>

ImageGraphicsView::ImageGraphicsView(QWidget *parent)
	: QGraphicsView(parent){
	_init();
}

ImageGraphicsView::ImageGraphicsView(QGraphicsScene *scene, QWidget *parent)
	: QGraphicsView(scene, parent){
	_init();
}

QGraphicsPixmapItem *ImageGraphicsView::addImage(const QImage &image)
{
	QPixmap pixmap = QPixmap::fromImage(image);
	return scene()->addPixmap(pixmap);
}

void ImageGraphicsView::enterEvent(QEvent *event)
{
	QGraphicsView::enterEvent(event);
	viewport()->setCursor(Qt::CrossCursor);
}

void ImageGraphicsView::mousePressEvent(QMouseEvent *event)
{
	QGraphicsView::mousePressEvent(event);
	viewport()->setCursor(Qt::OpenHandCursor);
}

void ImageGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
	QGraphicsView::mouseReleaseEvent(event);
	viewport()->setCursor(Qt::CrossCursor);
}

void ImageGraphicsView::mouseMoveEvent(QMouseEvent *event)
{

	QGraphicsView::mouseMoveEvent(event);
	QPointF pos = mapToScene(event->pos());
//	QGraphicsSceneMouseEvent* e = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
	emit mouseMove(pos);
}

void ImageGraphicsView::_init()
{
	auto* gz = new GraphicsViewZoom(this);
	gz->set_modifiers(Qt::NoModifier);
}
