#ifndef VMAPACCESS_H
#define VMAPACCESS_H

#include "vmap.h"
#include "../resourceaccess.h"

class VmapAccess: public vangers::AbstractResourceAccess<Vmap>
{
public:
	VmapAccess(int sizeX, int sizeY,
			   const vangers::Palette& palette,
			   const std::vector<std::pair<int, int>>& terrainColorOffsets)
        : _sizeX(sizeX)
        , _sizeY(sizeY)
		, _palette(palette)
		, _terrainColorOffsets(terrainColorOffsets)
    {}

    QSharedPointer<Vmap> read(QIODevice &device);
    void write(const QSharedPointer<Vmap> &resource, QIODevice &device);

private:
    int _sizeX;
    int _sizeY;
	vangers::Palette _palette;
	std::vector<std::pair<int, int>> _terrainColorOffsets;
};

#endif // VMAPACCESS_H
