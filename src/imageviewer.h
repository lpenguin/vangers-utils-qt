#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QSharedPointer>
#include <QSettings>
#include "image/image.h"
#include "image/palette.h"

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();


    void setImage(const QString& filename);
    void exportImage();
public slots:
    void handlePaletteChanged(QString paletteName);
private slots:
    void palette_useTransparentColor(bool use, int colorIndex);
private:
    Ui::ImageViewer *ui;
    QSharedPointer<vangers::Image> _image;
    vangers::Palette _palette;
    QList<QWidget*> _metaFields;
    QList<QSharedPointer<vangers::AbstractImageAccess>> _accesses;

    QSharedPointer<vangers::Image> tryRead(const QString& fileName);
    void setPalette(const vangers::Palette palette);
    void updateImage();
    bool useTransparentColor;
    int transparentColor;
    QString _filename;
    QSettings _settings;
};

#endif // IMAGEVIEWER_H
