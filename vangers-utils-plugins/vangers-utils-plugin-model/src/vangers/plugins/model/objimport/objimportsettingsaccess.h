#ifndef OBJIMPORTSETTINGSACCESS_H
#define OBJIMPORTSETTINGSACCESS_H

#include <vangers/core/plugin/resourceaccess.h>

#include "objimportsettings.h"

namespace vangers::plugins::model::objimport {
	class ObjImportSettingsAccess : public vangers::core::plugin::AbstractResourceAccess<ObjImportSettings>
	{
	public:
		ObjImportSettingsAccess() = default;
		void write(const ObjImportSettings& resource, QIODevice& device);
		bool read(ObjImportSettings& resource, QIODevice& device);
	};
}


#endif // OBJIMPORTSETTINGSACCESS_H
