#ifndef MODELVIEWERPLUGIN_H
#define MODELVIEWERPLUGIN_H

#include <plugins/resourceviewer.h>



class ModelViewerPlugin : public ResourceViewerPlugin
{
public:
	ModelViewerPlugin() = default;
	virtual ~ModelViewerPlugin() = default;

	QList<ResourceType> supportedImportTypes() const;
	QList<ResourceType> supportedExportTypes() const;
	ResourceViewer *makeResourceViewer(QWidget *parent);

	QString name() const {
		return "Model";
	};

	static const ResourceType M3D;
	static const ResourceType A3D;
	static const ResourceType Json;
	static const ResourceType Obj;
};

#endif // MODELVIEWERPLUGIN_H
