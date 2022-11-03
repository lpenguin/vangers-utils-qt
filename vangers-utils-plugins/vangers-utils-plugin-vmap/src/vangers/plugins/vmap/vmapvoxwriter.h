#ifndef VMAPVOXWRITER_H
#define VMAPVOXWRITER_H
#include "vmap.h"

#include <vangers/core//plugin/resourceaccess.h>

namespace vangers::plugins::vmap {

	struct VmapVoxWriterSettings{
		bool hollow;
		int32_t hollowSize;
		int32_t indexColorShift;
		bool compressChunks;
		bool flipX;
		int32_t voxSizeX;
		int32_t voxSizeY;
		int32_t chunkSizeX;
		int32_t chunkSizeY;
		float heightScale;
		std::map<int32_t, int32_t> indexColorMapping;

		static VmapVoxWriterSettings makeDefault();
	};

	class VmapVoxWriter
	{
	public:
		void write(const Vmap &resource, QString filename, const VmapVoxWriterSettings& settings);
	};

}
#endif // VMAPVOXWRITER_H
