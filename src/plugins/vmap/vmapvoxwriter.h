#ifndef VMAPVOXWRITER_H
#define VMAPVOXWRITER_H
#include "vmap.h"

#include <plugins/resourceaccess.h>

namespace vangers {

class VmapVoxWriter
{
public:
	void write(const Vmap &resource, QString filename);
};

}
#endif // VMAPVOXWRITER_H
