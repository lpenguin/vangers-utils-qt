#ifndef RESOURCEVIEWER_H
#define RESOURCEVIEWER_H

#include <QWidget>


struct ResourceType {
    QString name;
    QStringList extensions;
};

class ResourceViewerPluginInterface;

class ResourceViewer : public QWidget
{
    Q_OBJECT
public:
	explicit ResourceViewer(ResourceViewerPluginInterface* plugin, QWidget *parent = nullptr);
    virtual ~ResourceViewer(){}
    virtual bool importResource(const QString& filePath, const ResourceType& resourceType) = 0;
    virtual void exportResource(const QString& filePath, const ResourceType& resourceType) = 0;
    virtual QString currentFile() const = 0;
	ResourceViewerPluginInterface* plugin() const;
private:
	ResourceViewerPluginInterface* _plugin;
};

class ResourceViewerPluginInterface {
public:

	virtual ~ResourceViewerPluginInterface() = default;
    virtual QList<ResourceType> supportedImportTypes() const = 0;
    virtual QList<ResourceType> supportedExportTypes() const = 0;
    virtual ResourceViewer* makeResourceViewer(QWidget* parent = nullptr) = 0;
    virtual QString name() const = 0;
};

QT_BEGIN_NAMESPACE
#define ResourceViewerPluginInterface_iid "com.lilacpenguin.VangersUtils.ResourceViewerPluginInterface/1.0"
Q_DECLARE_INTERFACE(ResourceViewerPluginInterface, ResourceViewerPluginInterface_iid)
QT_END_NAMESPACE
#endif // RESOURCEVIEWER_H
