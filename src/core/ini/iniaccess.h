#ifndef INIACCESS_H
#define INIACCESS_H

#include <core/ini/inisettings.h>
#include <plugins/resourceaccess.h>

namespace vangers::core::ini {
	class IniAccess: public AbstractResourceAccess<IniSettings>
	{
	public:
		void write(const IniSettings& settings, QIODevice& device);
		bool read(IniSettings& settings, QIODevice& device);
	};

}

#endif // INIACCESS_H
