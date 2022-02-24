#ifndef MODELACCESS_H
#define MODELACCESS_H

#include <plugins/resourceaccess.h>
#include "model.h"


class ModelM3DAccess : public vangers::AbstractResourceAccess<model::M3D>
{
public:

	void write(const model::M3D &resource, QIODevice &device);

	bool read(model::M3D &resource, QIODevice &device);
};


class ModelA3DAccess : public vangers::AbstractResourceAccess<model::A3D>
{
public:

	void write(const model::A3D &resource, QIODevice &device);

	bool read(model::A3D &resource, QIODevice &device);
};

#endif // MODELACCESS_H
