#ifndef OBJEXPORTER_H
#define OBJEXPORTER_H

#include <plugins/model/m3d/m3d.h>
#include <plugins/model/obj/obj.h>
#include <plugins/model/objimport/objimportsettings.h>

namespace vangers::model::objimport {
	using namespace vangers::model::obj;
	using namespace vangers::model::m3d;

	class ObjExporter
	{
	public:
		void export_(const M3D& m3d, ObjectCollection& objects, ObjImportSettings& settings);
	};
}



#endif // OBJEXPORTER_H
