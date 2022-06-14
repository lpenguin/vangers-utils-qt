#ifndef OBJIMPORTSETTINGSACCESS_H
#define OBJIMPORTSETTINGSACCESS_H

#include <plugins/resourceaccess.h>
#include <plugins/model/objimport/objimportsettings.h>

namespace vangers::model::objimport {
	class ObjImportSettingsAccess : public AbstractResourceAccess<ObjImportSettings>
	{
	public:
		ObjImportSettingsAccess() = default;
		void write(const ObjImportSettings& resource, QIODevice& device);
		bool read(ObjImportSettings& resource, QIODevice& device);
	};
}


#endif // OBJIMPORTSETTINGSACCESS_H
