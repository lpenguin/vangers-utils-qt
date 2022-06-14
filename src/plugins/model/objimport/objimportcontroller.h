#ifndef OBJIMPORTCONTROLLER_H
#define OBJIMPORTCONTROLLER_H

#include <memory>
#include "objimportsettingswidget.h"
#include <plugins/model/obj/obj.h>
#include <plugins/model/m3d/m3d.h>

namespace vangers::model::objimport {
	using namespace vangers::model::obj;
	using namespace vangers::model::m3d;

	class ObjImportController
	{
	public:
		ObjImportController(ObjImportSettingsWidget* widget);

		void setObjectCollection(
				std::unique_ptr<ObjectCollection>&& objectCollection,
				std::unique_ptr<ObjImportSettings>&& importSettings);

		bool import(M3D& m3d);

		const std::unique_ptr<ObjImportSettings>&
		importSettings() const {
			return _importSettings;
		}
	private:
		ObjImportSettingsWidget* _widget;
		std::unique_ptr<ObjectCollection> _objectCollection;
		std::unique_ptr<ObjImportSettings> _importSettings;
	};

}


#endif // OBJIMPORTCONTROLLER_H
