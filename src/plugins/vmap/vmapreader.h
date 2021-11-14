#ifndef VMAPREADER_H
#define VMAPREADER_H

#include "vmap.h"
#include "../resourceaccess.h"
namespace vangers {
class VmapReader
{
public:
	bool read(Vmap& vmap, QFile &iniFile);
};
}


#endif // VMAPREADER_H
