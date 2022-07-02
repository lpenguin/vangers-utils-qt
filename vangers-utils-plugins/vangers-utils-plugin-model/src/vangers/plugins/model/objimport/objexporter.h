#ifndef OBJEXPORTER_H
#define OBJEXPORTER_H

#include <m3d/m3d.h>
#include <obj/obj.h>
#include <vangers/plugins/model/objimport/objimportsettings.h>

namespace vangers::plugins::model::objimport {
	class ObjExporter
	{
	public:
		void export_(const m3d::M3D& m3d, obj::ObjectCollection& objects, ObjImportSettings& settings);
	};
}



#endif // OBJEXPORTER_H
