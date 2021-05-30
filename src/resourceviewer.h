#ifndef RESOURCEVIEWER_H
#define RESOURCEVIEWER_H

#include <QWidget>


struct ResourceType {
    QString name;
    QStringList extensions;
};

class ResourceViewerPlugin;

class ResourceViewer : public QWidget
{
    Q_OBJECT
public:
    explicit ResourceViewer(ResourceViewerPlugin* plugin, QWidget *parent = nullptr);
    virtual ~ResourceViewer(){}
    virtual void importResource(const QString& filePath, const ResourceType& resourceType) = 0;
    virtual void exportResource(const QString& filePath, const ResourceType& resourceType) = 0;
    virtual QString currentFile() const = 0;
    ResourceViewerPlugin* plugin() const;
private:
    ResourceViewerPlugin* _plugin;
};

class ResourceViewerPlugin: public QObject {
public:
    explicit ResourceViewerPlugin(QObject *parent = nullptr)
        : QObject(parent) {}

    virtual ~ResourceViewerPlugin(){}
    virtual QList<ResourceType> supportedImportTypes() const = 0;
    virtual QList<ResourceType> supportedExportTypes() const = 0;
    virtual ResourceViewer* makeResourceViewer(QWidget* parent = nullptr) = 0;
    virtual QString name() const = 0;
};

#endif // RESOURCEVIEWER_H
