#ifndef HEIGHTMAPFROMMESH_H
#define HEIGHTMAPFROMMESH_H

#include <QString>
#include <vangers/core/vector/vector.h>
#include <vangers/core/error/error.h>
#include <vangers/plugins/vmap/vmapparams.h>

namespace vangers::model2vmap {
	enum class DoubleLevelStrategy {
		TakeMax,
		DetermineMid,
	};

	struct RasterSettings {
		vangers::core::vector::Vector3I32 worldSize;
		bool writeVpr;
		vangers::plugins::vmap::VmapGlobalParams globalParams;
		int32_t hThr;
		DoubleLevelStrategy doubleLevelStrategy;
		int32_t minUpDownLevelDist;
		int32_t minDoubleLevelThikness;
		float shadowAspect;
	};

	vangers::core::error::Result heightmapFromMesh(QString objPath, QString outputDir, const RasterSettings& settings);
}

#endif // HEIGHTMAPFROMMESH_H
