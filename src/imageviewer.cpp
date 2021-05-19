#include "imageviewer.h"
#include "ui_imageviewer.h"

#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QStringListModel>
#include <QScopedPointer>

#include "image/bmpimage.h"
#include "image/xbmimage.h"
#include "image/pngimage.h"


ImageViewer::ImageViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageViewer),
    _image(nullptr),
    _palette(vangers::Palette::grayscale()),
    _accesses({
                 QSharedPointer<vangers::BmpImage1>::create(),
                 QSharedPointer<vangers::BmpImage2>::create(),
                 QSharedPointer<vangers::BmpImage3>::create(),
                 QSharedPointer<vangers::BmpImage4>::create(),
                 QSharedPointer<vangers::XbmImageAccess>::create(),
                 QSharedPointer<vangers::PngImageAccess>::create(),
                 }),
    useTransparentColor(false),
    transparentColor(0),
    _settings("lpenguin", "Vangers Resource Explorer")
{
    ui->setupUi(this);
    QObject::connect(ui->paletteViewer, &PaletteViewer::useTransparentColor,
                     this, &ImageViewer::palette_useTransparentColor);
    _palette = vangers::Palette::read(ui->paletteViewer->currentPalette());
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

QSharedPointer<vangers::Image> ImageViewer::tryRead(const QString& fileName){
    QFile f(fileName);
    f.open(QFile::ReadOnly);

    for(auto& access: _accesses){
        auto image = access->read(f);
        f.seek(0);
        if(!image.isNull()){
            return image;
        }
    }
    qWarning() << "Cannot find a sutable reader for"<<fileName;
    return QSharedPointer<vangers::Image>();
}


void ImageViewer::setImage(const QString& filename)
{
    _image = tryRead(filename);
    _filename = filename;
    updateImage();
}

void ImageViewer::setPalette(const vangers::Palette palette)
{
    _palette = palette;
    updateImage();
}

void ImageViewer::exportImage()
{
    QString vbmpFilter = "Vangers BMP (*.bmp *bmo)";
    QString pngFilter = "PNG-8 (*.png)";
    QString xbmFilter = "Vangers XBM (*.xbm)";

    QString filters = (QStringList() << pngFilter << vbmpFilter << xbmFilter).join(";;");

    QString selectedFilter;

    auto filename = QFileDialog::getSaveFileName(
                this,
                tr("Save file"),
                 QFileInfo(_filename).baseName(),
                filters,
                &selectedFilter);


    QScopedPointer<vangers::AbstractImageAccess> access {nullptr};

    if(selectedFilter == pngFilter){
        access.reset(new vangers::PngImageAccess());
    }

    if(selectedFilter == xbmFilter){
        access.reset(new vangers::XbmImageAccess());
    }

    if(selectedFilter == vbmpFilter){
        // TODO: don't need to be sucj specific
        access.reset(new vangers::BmpImage1());
    }

    if(access.isNull()){
        return;
    }

    QFile file(filename);
    file.open(QFile::WriteOnly);
    _image->image()->setColorTable(_palette);
    access->write(_image, file);
}

void ImageViewer::handlePaletteChanged(QString paletteName)
{
    _palette = vangers::Palette::read(paletteName);
    updateImage();
}

void ImageViewer::palette_useTransparentColor(bool use, int colorIndex)
{
    useTransparentColor = use;
    transparentColor = colorIndex;
    updateImage();
}

void ImageViewer::updateImage()
{
    if(_image.isNull()){
        return;
    }

    QGraphicsScene* scene = new QGraphicsScene(this);
    auto& i = *(_image->image());
//    ui->inageTypeLabel->setText(_image->typeName());

    vangers::Palette palette = vangers::Palette(_palette);
    if(useTransparentColor && transparentColor >= 0 && transparentColor < palette.size()){
        palette[transparentColor] = qRgba(0, 0, 0, 0);
    }
    i.setColorTable(palette);

    QPixmap pixmap = QPixmap::fromImage(i);
    scene->addPixmap(pixmap);
    qDebug() << i.rect() << _image->image()->rect() << _image->image();
    scene->setSceneRect(i.rect());
    ui->imageWidget->setScene(scene);

    QFormLayout* layout = (QFormLayout*)ui->infoForm->layout();

    for(auto* metaField: _metaFields){
        layout->removeRow(metaField);
    }
    _metaFields.clear();

    auto meta = _image->meta();
    for(auto& field: meta->keys()){
        auto value = (*meta)[field];
        QLabel* label = new QLabel(ui->infoForm);
        label->setText(QString("%1").arg(value));
        _metaFields.append(label);
        layout->addRow(vangers::toString(field), label);
    }

    repaint();
}
