#ifndef OBJIMPORTCONTROLLER_H
#define OBJIMPORTCONTROLLER_H

#include <memory>
#include "objimportsettingswidget.h"
#include <obj/obj.h>
#include <m3d/m3d.h>

namespace vangers::plugins::model::objimport {
	class ObjImportController
	{
	public:
		ObjImportController(ObjImportSettingsWidget* widget);

		void setObjectCollection(
				std::unique_ptr<obj::ObjectCollection>&& objectCollection,
				std::unique_ptr<ObjImportSettings>&& importSettings);

		bool import(m3d::M3D& m3d);

		const std::unique_ptr<ObjImportSettings>&
		importSettings() const {
			return _importSettings;
		}
	private:
		ObjImportSettingsWidget* _widget;
		std::unique_ptr<obj::ObjectCollection> _objectCollection;
		std::unique_ptr<ObjImportSettings> _importSettings;
	};

}


#endif // OBJIMPORTCONTROLLER_H
