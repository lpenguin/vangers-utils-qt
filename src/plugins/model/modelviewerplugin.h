#ifndef MODELVIEWERPLUGIN_H
#define MODELVIEWERPLUGIN_H

#include <plugins/resourceviewer.h>



class ModelViewerPlugin : public ResourceViewerPlugin
{
public:
	ModelViewerPlugin();
	virtual ~ModelViewerPlugin() = default;

	QList<ResourceType> supportedImportTypes() const;
	QList<ResourceType> supportedExportTypes() const;
	ResourceViewer *makeResourceViewer(QWidget *parent);

	QString name() const {
		return "Model";
	};
private:
	static const ResourceType M3D;
};

#endif // MODELVIEWERPLUGIN_H
