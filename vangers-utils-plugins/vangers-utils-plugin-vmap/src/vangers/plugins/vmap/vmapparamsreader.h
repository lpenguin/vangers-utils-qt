#ifndef VMAPPARAMSREADER_H
#define VMAPPARAMSREADER_H

#include <vangers/plugins/vmap/vmapparams.h>

#include <QString>

namespace vangers::plugins::vmap {
	class VmapParamsReader
	{
		public:
			VmapParamsReader() = default;
			void readParams(const QString& fileName, VmapParams& params);
	};

}

#endif // VMAPPARAMSREADER_H
