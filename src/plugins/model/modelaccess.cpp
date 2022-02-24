#include "modelaccess.h"
#include <binaryio.h>
#include "modelbinaryreaderext.h"

void ModelM3DAccess::write(const model::M3D &resource, QIODevice &device)
{

}

bool ModelM3DAccess::read(model::M3D &resource, QIODevice &device)
{
	vangers::BinaryReader reader(&device);
	return reader.tryRead(resource);
}

void ModelA3DAccess::write(const model::A3D& resource, QIODevice& device)
{

}

bool ModelA3DAccess::read(model::A3D& resource, QIODevice& device)
{
	vangers::BinaryReader reader(&device);
	return reader.tryRead(resource);
}
