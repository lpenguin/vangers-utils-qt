#include "objimportcontroller.h"
#include <plugins/model/objimport/objimporter.h>

using namespace vangers::model::objimport;

ObjImportController::ObjImportController(ObjImportSettingsWidget* widget)
	: _widget(widget)
	, _objectCollection()
	, _importSettings()
{

}

void ObjImportController::setObjectCollection(
		std::unique_ptr<ObjectCollection>&& objectCollection,
		std::unique_ptr<ObjImportSettings>&& importSettings)
{
	_importSettings = std::move(importSettings);
	_objectCollection = std::move(objectCollection);
	_widget->setImportSettings(*_importSettings);
}

bool ObjImportController::import(M3D& m3d)
{
	*_importSettings = {};
	_widget->makeImportSettings(*_importSettings);
	return ObjImporter().import(*_objectCollection,
								*_importSettings,
								m3d);
}
