#ifndef PALETTEVIEWER_H
#define PALETTEVIEWER_H

#include <QMap>

#include "plugins/resourceviewer.h"
#include "plugins/palette/microsoftpalaccess.h"
#include "plugins/palette/vangerspaletteaccess.h"
#include "plugins/palette/pngpaletteaccess.h"

using namespace vangers;
namespace Ui {
    class PaletteViewer;
}

class PaletteViewer : public ResourceViewer
{
    Q_OBJECT

public:
    ~PaletteViewer();
public:
    explicit PaletteViewer(ResourceViewerPlugin* plugin, QWidget *parent = nullptr);

    bool importResource(const QString& filePath, const ResourceType& resourceType) override;
    void exportResource(const QString& filePath, const ResourceType& resourceType) override;
    QString currentFile() const override;

public slots:
    void importButton_click();
private:
    QString _currentFile;
    static const QMap<QString, QSharedPointer<AbstractResourceAccess<Palette>>> accesses;
private:
    Ui::PaletteViewer *ui;
};

class PaletteViewerPlugin: public ResourceViewerPlugin {


    // ResourceViewerPlugin interface
public:
    static const ResourceType PalType;
    static const ResourceType PngType;
    static const ResourceType MicrosoftPalType;

    explicit PaletteViewerPlugin(QObject* parent = nullptr)
        : ResourceViewerPlugin(parent) {}

    QList<ResourceType> supportedImportTypes() const override;
    QList<ResourceType> supportedExportTypes() const override;
    ResourceViewer* makeResourceViewer(QWidget* parent) override;
    QString name() const override {
        return "Palette";
    }
};

#endif // PALETTEVIEWER_H
