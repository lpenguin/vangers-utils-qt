#include "modelaccess.h"
#include <binaryio.h>
#include "modelbinaryreaderext.h"
#include "modelbinarywriterext.h"

using namespace vangers::model::m3d;

void M3DAccess::write(const M3D &resource, QIODevice &device)
{
	vangers::BinaryWriter writer(&device);
	writer.write(resource);
}

bool M3DAccess::read(M3D &resource, QIODevice &device)
{
	vangers::BinaryReader reader(&device);
	return reader.tryRead(resource);
}

void A3DAccess::write(const A3D& resource, QIODevice& device)
{
	vangers::BinaryWriter writer(&device);
	writer.write(resource);
}

bool A3DAccess::read(A3D& resource, QIODevice& device)
{
	vangers::BinaryReader reader(&device);
	return reader.tryRead(resource);
}
