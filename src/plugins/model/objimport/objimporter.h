#ifndef OBJIMPORTER_H
#define OBJIMPORTER_H

#include <plugins/model/objimport/objimportsettings.h>
#include <plugins/model/m3d/m3d.h>
#include <plugins/model/obj/obj.h>
#include <QSharedPointer>

namespace vangers::model::objimport {
	using namespace vangers::model::m3d;
	using namespace vangers::model::obj;

	class ObjImporter
	{
	public:
		bool import(const ObjectCollection& objCollection,
					const ObjImportSettings& importSettings,
					M3D& m3d);

		static void makeDefaultImportSettings(
				const ObjectCollection& objCollection,
				ObjImportSettings& importSettings);

	};

}



#endif // OBJIMPORTER_H
