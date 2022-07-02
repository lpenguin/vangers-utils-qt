#include "vmapvmpwriter.h"

#include <QDebug>
#include <vangers/core/binaryio.h>

using namespace vangers::plugins::vmap;
using namespace vangers::core::binario;

void VmapVmpWriter::write(const Vmap& vmap, QIODevice& device)
{
	BinaryWriter writer(&device);

	const uint8_t* heightData = vmap.heightConst().data();
	const uint8_t* metaData = vmap.metaConst().data();
	const int height = vmap.size().height();
	const int width = vmap.size().width();

	qDebug() << width << height;
	for(int iRow = 0; iRow < height; iRow++){
		for(int iCol = 0; iCol < width; iCol++){
			writer.write<uint8_t>(heightData[iRow * width + iCol]);
		}

		for(int iCol = 0; iCol < width; iCol++){
			writer.write<uint8_t>(metaData[iRow * width + iCol]);
		}
	}
}
