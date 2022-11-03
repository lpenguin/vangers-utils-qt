#ifndef VMAPPARAMS_H
#define VMAPPARAMS_H

#include <QString>
#include <stdint.h>

namespace vangers::plugins::vmap {
	struct VmapStorageParams {
		QString levelBaseName;
		bool isCompressed;
	};

	struct VmapGlobalParams {
		int32_t mapPowerX;
		int32_t mapPowerY;
		int32_t geoNetPower;
		int32_t sectionSizePower;
		int32_t minSquarePower;
	};

	struct VmapParams {
		VmapStorageParams storage;
		VmapGlobalParams global;
	};
}
#endif // VMAPPARAMS_H
