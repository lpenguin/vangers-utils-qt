#include "modelviewerplugin.h"
#include "modelviewer.h"
const ResourceType ModelViewerPlugin::M3D {
	.name = "M3D (3D model)",
	.extensions = {
		"*.m3d"
	}
};

const ResourceType ModelViewerPlugin::A3D {
	.name = "A3D (Animated 3D model)",
	.extensions = {
		"*.a3d"
	}
};


const ResourceType ModelViewerPlugin::Json {
	.name = "JSON (3D Model/ Animated 3D Model)",
	.extensions = {
		"*.json"
	}
};

const ResourceType ModelViewerPlugin::Obj {
	.name = "Obj (3D Model/ Animated 3D Model)",
	.extensions = {
		"*.obj"
	}
};

ModelViewerPlugin::ModelViewerPlugin()
{
}

QList<ResourceType> ModelViewerPlugin::supportedImportTypes() const
{
	return {M3D, A3D, Json};
}

QList<ResourceType> ModelViewerPlugin::supportedExportTypes() const
{
	return {Json};
}

ResourceViewer *ModelViewerPlugin::makeResourceViewer(QWidget *parent)
{
	return new ModelViewer(this, parent);
}
