#include "modelviewerplugin.h"
#include "modelviewer.h"

#include "drawdata.h"
#include <QQmlEngine>

const ResourceType ModelViewerPlugin::M3D {
	.name = "M3D",
	.extensions = {
		"*.m3d"
	}
};

ModelViewerPlugin::ModelViewerPlugin()
{
	qmlRegisterType<DrawData>("vangers_utils", 1, 0, "DrawData");
}

QList<ResourceType> ModelViewerPlugin::supportedImportTypes() const
{
	return {M3D};
}

QList<ResourceType> ModelViewerPlugin::supportedExportTypes() const
{
	return {};
}

ResourceViewer *ModelViewerPlugin::makeResourceViewer(QWidget *parent)
{
	return new ModelViewer(this, parent);
}
