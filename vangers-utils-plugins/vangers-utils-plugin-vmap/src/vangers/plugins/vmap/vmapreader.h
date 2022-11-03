#ifndef VMAPREADER_H
#define VMAPREADER_H

#include "vmap.h"

#include <QFile>
namespace vangers::plugins::vmap {
class VmapReader
{
public:
	bool read(Vmap& vmap, QFile &iniFile);
};
}


#endif // VMAPREADER_H
