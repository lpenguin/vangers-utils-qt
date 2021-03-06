#include "modelviewerplugin.h"
#include "modelviewer.h"

using namespace vangers::model::view;

const ResourceType ModelViewerPlugin::M3D {
	.name = "M3D",
	.extensions = {
		"*.m3d"
	}
};

const ResourceType ModelViewerPlugin::A3D {
	.name = "A3D",
	.extensions = {
		"*.a3d"
	}
};


const ResourceType ModelViewerPlugin::Json {
	.name = "JSON",
	.extensions = {
		"*.json"
	}
};

const ResourceType ModelViewerPlugin::Obj {
	.name = "Wavefront OBJ",
	.extensions = {
		"*.obj"
	}
};

QList<ResourceType> ModelViewerPlugin::supportedImportTypes() const
{
	return {M3D, Obj, Json};
}

QList<ResourceType> ModelViewerPlugin::supportedExportTypes() const
{
	return {M3D, Obj, Json};
}

ResourceViewer *ModelViewerPlugin::makeResourceViewer(QWidget *parent)
{
	return new ModelViewer(this, parent);
}
