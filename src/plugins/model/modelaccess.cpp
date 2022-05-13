#include "modelaccess.h"
#include <binaryio.h>
#include "modelbinaryreaderext.h"

void ModelAccess::write(const model::M3D &resource, QIODevice &device)
{

}

bool ModelAccess::read(model::M3D &resource, QIODevice &device)
{
	vangers::BinaryReader reader(&device);
	return reader.tryRead(resource);
}
