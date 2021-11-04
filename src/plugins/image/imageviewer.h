#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QSharedPointer>
#include <QSettings>
#include "image/image.h"
#include "image/palette.h"
#include "plugins/resourceviewer.h"

namespace Ui {
class ImageViewer;
}

class ImageViewerPlugin: public ResourceViewerPlugin {
public:
    static const ResourceType PngType;
    static const ResourceType XbmType;
    static const ResourceType vBmpType;
    static const ResourceType TiffType;
    static const ResourceType SvsType;
    static const ResourceType MssType;

    explicit ImageViewerPlugin(QObject *parent = nullptr)
        : ResourceViewerPlugin(parent) {}

    QList<ResourceType> supportedImportTypes() const override;
    QList<ResourceType> supportedExportTypes() const override;
    ResourceViewer* makeResourceViewer(QWidget* parent = nullptr) override;

    QString name() const override {
        return "Image";
    }
};

class ImageViewer : public ResourceViewer
{
    Q_OBJECT

public:
    explicit ImageViewer(ImageViewerPlugin* plugin, QWidget *parent = nullptr);
    ~ImageViewer();


    bool importResource(const QString& filename, const ResourceType& resourceType) override;
    void exportResource(const QString& filename, const ResourceType& resourceType) override;
    QString currentFile() const override;
public slots:
    void handlePaletteChanged(QString paletteName);
private slots:
    void palette_useTransparentColor(bool use, int colorIndex);
private:
    Ui::ImageViewer *ui;
    QSharedPointer<vangers::Image> _image;
    vangers::Palette _palette;
    QList<QWidget*> _metaFields;
    QMap<QString, QSharedPointer<vangers::AbstractImageAccess>> _accesses;

    QSharedPointer<vangers::Image> tryRead(const QString& fileName, const ResourceType &resourceType);
    void setPalette(const vangers::Palette palette);
    void updateImage();
    bool useTransparentColor;
    int transparentColor;
    QString _filename;
    QSettings _settings;
};

#endif // IMAGEVIEWER_H
