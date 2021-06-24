#include "imageviewer.h"
#include "ui_imageviewer.h"

#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QStringListModel>
#include <QScopedPointer>
#include <QGraphicsPixmapItem>

#include "graphicsviewzoom.h"
#include "image/bmpimage.h"
#include "image/xbmimage.h"
#include "image/qtimageaccess.h"

const ResourceType ImageViewerPlugin::PngType = {
    .name = "PNG image",
    .extensions = {"*.png"}
};

const ResourceType ImageViewerPlugin::TiffType = {
    .name = "TIFF image",
    .extensions = {"*.tiff", "*.tif"}
};

const ResourceType ImageViewerPlugin::XbmType = {
    .name = "Vangers XBM image",
    .extensions = {"*.xbm"}
};

const ResourceType ImageViewerPlugin::vBmpType = {
    .name = "Vangers BMP image",
    .extensions = {"*.bmp", "*.bmo", "*.bml"}
};

ImageViewer::ImageViewer(ImageViewerPlugin* plugin, QWidget *parent) :
    ResourceViewer(plugin, parent),
    ui(new Ui::ImageViewer),
    _image(nullptr),
    _palette(vangers::Palette::grayscale()),
    _accesses({
                 {ImageViewerPlugin::vBmpType.name, QSharedPointer<vangers::BmpImageAccess>::create()},
                 {ImageViewerPlugin::XbmType.name, QSharedPointer<vangers::XbmImageAccess>::create()},
                 {ImageViewerPlugin::PngType.name, QSharedPointer<vangers::QtImageAccess>::create("png")},
                 {ImageViewerPlugin::TiffType.name, QSharedPointer<vangers::QtImageAccess>::create("tiff")},
                 }),
    useTransparentColor(false),
    transparentColor(0),
    _settings("lpenguin", "Vangers Resource Explorer")
{
    ui->setupUi(this);
    QObject::connect(ui->paletteViewer, &PaletteView::useTransparentColor,
                     this, &ImageViewer::palette_useTransparentColor);
    auto* gz = new GraphicsViewZoom(ui->imageWidget);
    gz->set_modifiers(Qt::NoModifier);
    _palette = vangers::Palette::read(ui->paletteViewer->currentPalette());
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

QSharedPointer<vangers::Image> ImageViewer::tryRead(const QString& fileName, const ResourceType& resourceType){
    QFile f(fileName);
    f.open(QFile::ReadOnly);

    if(!_accesses.contains(resourceType.name)){
        return {};
    }

    auto access = _accesses[resourceType.name];
    auto image = access->read(f);

    if(image.isNull()){
        qWarning() << "Cannot find a sutable reader for"<<fileName;
        return {};
    }

    return image;
}


bool ImageViewer::importResource(const QString& filename, const ResourceType& resourceType)
{
    _image = tryRead(filename, resourceType);
    if(_image.isNull()){
        return false;
    }
    _filename = filename;
    updateImage();
    return true;
}

void ImageViewer::setPalette(const vangers::Palette palette)
{
    _palette = palette;
    updateImage();
}

void ImageViewer::exportResource(const QString& filename, const ResourceType& resourceType)
{
    if(!_accesses.contains(resourceType.name)){
        return;
    }

    auto access = _accesses[resourceType.name];

    if(access.isNull()){
        return;
    }

    QFile file(filename);
    file.open(QFile::WriteOnly);
    _image->image()->setColorTable(_palette);
    access->write(_image, file);
}

QString ImageViewer::currentFile() const
{
    return _filename;
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
    auto* item = scene->addPixmap(pixmap);
    item->setTransformationMode(Qt::SmoothTransformation);
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

QList<ResourceType> ImageViewerPlugin::supportedImportTypes() const
{
    return {vBmpType, XbmType, PngType, TiffType};
}

QList<ResourceType> ImageViewerPlugin::supportedExportTypes() const
{
    return {vBmpType, XbmType, PngType, TiffType};
}

ResourceViewer* ImageViewerPlugin::makeResourceViewer(QWidget* parent)
{
    return new ImageViewer(this, parent);
}
