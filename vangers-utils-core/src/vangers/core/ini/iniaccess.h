#ifndef INIACCESS_H
#define INIACCESS_H

#include <vangers/core//ini/inisettings.h>
#include <vangers/core//plugin/resourceaccess.h>

namespace vangers::core::ini {
	namespace  {
		using namespace vangers::core::plugin;
	}

	class IniAccess: public AbstractResourceAccess<IniSettings>
	{
	public:
		void write(const IniSettings& settings, QIODevice& device);
		bool read(IniSettings& settings, QIODevice& device);
	};

}

#endif // INIACCESS_H
