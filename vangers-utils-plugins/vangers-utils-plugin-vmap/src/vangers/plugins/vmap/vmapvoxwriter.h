#ifndef VMAPVOXWRITER_H
#define VMAPVOXWRITER_H
#include "vmap.h"

#include <vangers/core//plugin/resourceaccess.h>

namespace vangers::plugins::vmap {

class VmapVoxWriter
{
public:
	void write(const Vmap &resource, QString filename);
};

}
#endif // VMAPVOXWRITER_H
