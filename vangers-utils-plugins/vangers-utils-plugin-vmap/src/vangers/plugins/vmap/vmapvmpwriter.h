#ifndef VMAPVMPWRITER_H
#define VMAPVMPWRITER_H

#include "vmap.h"

namespace vangers::plugins::vmap {
	class VmapVmpWriter
	{
	public:
		void write(const Vmap& vmap, QIODevice& device);
	};

}

#endif // VMAPVMPWRITER_H
