#include "qimagewidget.h"
#include <QDebug>
#include <QPainter>

void QImageWidget::paintEvent(QPaintEvent *event)
{
    qDebug() << "QImageWidget::paintEvent";
    QPainter painter(this);
    qDebug () << _image.pixel(10, 10) << _image.data_ptr();
    painter.drawImage(rect(), _image);
}
