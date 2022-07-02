#ifndef OBJIMPORTER_H
#define OBJIMPORTER_H

#include "objimportsettings.h"
#include <m3d/m3d.h>
#include <obj/obj.h>
#include <QSharedPointer>

namespace vangers::plugins::model::objimport {
	class ObjImporter
	{
	public:
		bool import(const obj::ObjectCollection& objCollection,
					const ObjImportSettings& importSettings,
					::m3d::M3D& m3);

		static void makeDefaultImportSettings(
				const obj::ObjectCollection& objCollection,
				ObjImportSettings& importSettings);

	};
}



#endif // OBJIMPORTER_H
