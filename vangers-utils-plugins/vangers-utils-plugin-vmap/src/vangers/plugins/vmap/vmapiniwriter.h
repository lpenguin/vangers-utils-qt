#ifndef VMAPINIWRITER_H
#define VMAPINIWRITER_H

#include "vmap.h"
#include "layer/abstractimagelayer.h"

namespace vangers::plugins::vmap {
class VmapIniWriter
{
public:
	void write(const Vmap &resource, QString filename, Level level = Level::Both);
};

}

#endif // VMAPINIWRITER_H
