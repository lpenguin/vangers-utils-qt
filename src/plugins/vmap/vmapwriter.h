#ifndef VMAPWRITER_H
#define VMAPWRITER_H

#include "vmap.h"

#include <plugins/resourceaccess.h>
#include <plugins/vmap/layer/abstractimagelayer.h>

namespace vangers {
class VmapWriter
{
public:
	void write(const Vmap &resource, QString filename, Level level = Level::Both);
};

}

#endif // VMAPWRITER_H
