#ifndef HEIGHTMAPFROMMESH_H
#define HEIGHTMAPFROMMESH_H

#include <QString>
#include <vangers/core/vector/vector.h>

namespace vangers::model2vmap {
	enum class DoubleLevelStrategy {
		TakeMax,
		DetermineMid,
	};

	struct RasterSettings {
		vangers::core::vector::Vector3I32 worldSize;
		int32_t hThr;
		DoubleLevelStrategy doubleLevelStrategy;
	};

	bool heightmapFromMesh(QString objPath, QString outputDir, const RasterSettings& settings);
}

#endif // HEIGHTMAPFROMMESH_H
