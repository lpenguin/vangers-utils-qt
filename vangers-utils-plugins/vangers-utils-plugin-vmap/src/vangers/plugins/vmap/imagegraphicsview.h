#ifndef IMAGEGRAPHICSVIEW_H
#define IMAGEGRAPHICSVIEW_H

#include <QGraphicsView>

class ImageGraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	ImageGraphicsView(QWidget* parent=nullptr);

	ImageGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);

	QGraphicsPixmapItem* addImage(const QImage& image);
signals:
	void mouseMove(QPointF pos);

protected:
	void enterEvent(QEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private:
	void _init();

};

#endif // IMAGEGRAPHICSVIEW_H
