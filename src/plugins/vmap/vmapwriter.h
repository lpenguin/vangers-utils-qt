#ifndef VMAPWRITER_H
#define VMAPWRITER_H

#include "vmap.h"

#include <plugins/resourceaccess.h>


namespace vangers {
class VmapWriter
{
public:
	void write(const Vmap &resource, QString filename);
};

}

#endif // VMAPWRITER_H
