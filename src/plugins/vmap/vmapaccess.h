#ifndef VMAPACCESS_H
#define VMAPACCESS_H

#include "vmap.h"
#include "../resourceaccess.h"

class VmapAccess: public vangers::AbstractResourceAccess<Vmap>
{
public:
    VmapAccess(int sizeX, int sizeY)
        : _sizeX(sizeX)
        , _sizeY(sizeY)
    {}

    QSharedPointer<Vmap> read(QIODevice &device);
    void write(const QSharedPointer<Vmap> &resource, QIODevice &device);

private:
    int _sizeX;
    int _sizeY;
};

#endif // VMAPACCESS_H
