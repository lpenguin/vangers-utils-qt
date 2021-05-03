#ifndef QIMAGEWIDGET_H
#define QIMAGEWIDGET_H

#include <QImage>
#include <QWidget>


class QImageWidget: public QWidget
{
public:
    QImageWidget(QWidget* parent = nullptr): QWidget(parent) {}
    void paintEvent(QPaintEvent *event) override;
    void setImage(const QImage& image){
        _image = image;
    }
private:
    QImage _image;
};

#endif // QIMAGEWIDGET_H
