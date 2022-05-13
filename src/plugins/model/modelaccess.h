#ifndef MODELACCESS_H
#define MODELACCESS_H

#include <plugins/resourceaccess.h>
#include "model.h"


class ModelAccess : public vangers::AbstractResourceAccess<model::M3D>
{
public:

	void write(const model::M3D &resource, QIODevice &device);

	bool read(model::M3D &resource, QIODevice &device);
};

#endif // MODELACCESS_H
