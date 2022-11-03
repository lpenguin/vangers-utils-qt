#include <vangers/core/binaryio.h>

#include "modelaccess.h"
#include "modelbinaryreaderext.h"
#include "modelbinarywriterext.h"

using namespace m3d;
using namespace vangers::core::binario;

void M3DAccess::write(const M3D &resource, QIODevice &device)
{
	BinaryWriter writer(&device);
	writer.write(resource);
}

bool M3DAccess::read(M3D &resource, QIODevice &device)
{
	BinaryReader reader(&device);
	return reader.tryRead(resource);
}

void A3DAccess::write(const A3D& resource, QIODevice& device)
{
	BinaryWriter writer(&device);
	writer.write(resource);
}

bool A3DAccess::read(A3D& resource, QIODevice& device)
{
	BinaryReader reader(&device);
	return reader.tryRead(resource);
}
