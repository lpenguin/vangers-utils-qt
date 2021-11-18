#ifndef VMAPVIEWERPLUGIN_H
#define VMAPVIEWERPLUGIN_H

#include "vmap.h"

#include <plugins/resourceviewer.h>

#include <QGraphicsItem>
#include "layer/abstractimagelayer.h"


namespace Ui {
class VmapViewer;
}

class VmapViewerPlugin : public ResourceViewerPlugin
{
public:
    static const ResourceType VmcType;
    static const ResourceType LevelType;
    explicit VmapViewerPlugin(QObject *parent = nullptr):
        ResourceViewerPlugin(parent)
    {

    }

    // ResourceViewerPlugin interface
public:
    QList<ResourceType> supportedImportTypes() const {
        return {VmcType};
    };

    QList<ResourceType> supportedExportTypes() const {
        return {LevelType};
    };

    ResourceViewer *makeResourceViewer(QWidget *parent);

    QString name() const {
        return "Vmap";
    };
};

class VmapViewer: public ResourceViewer
{
    Q_OBJECT

public:
    explicit VmapViewer(VmapViewerPlugin* plugin, QWidget *parent = nullptr);
    ~VmapViewer();


    bool importResource(const QString& filename, const ResourceType& resourceType) override;
    void exportResource(const QString& filename, const ResourceType& resourceType) override;
    QString currentFile() const override;
private:
    Ui::VmapViewer* _ui;
    QString _currentFile;
    QSharedPointer<Vmap> _vmap;
	QGraphicsPixmapItem* _heightItem;
    QGraphicsPixmapItem* _metaItem;

	void applyMask(QString layerName);

    const uint8_t DeltaMask = 0b00000011;
    const uint8_t ObjShadowMask = 0b00000100;
    const uint8_t TerrainTypeMask = 0b00111000;
    const uint8_t DoubleLevelMask = 0b01000000;
    const uint8_t ShadowMask = 0b10000000;

	QMap<QString, AbstractImageLayer*> _layers;
	Level getLevel();
private slots:
    void onHeightToggled(bool checked);
    void onMetaToggled(bool checked);
	void onMaskTypeChanged(int maskIndex);
	void onMapMouseMove(QPointF pos);
	void onMixValueChanged(int value);
	void onLevelChanged(int index);
};

#endif // VMAPVIEWERPLUGIN_H

