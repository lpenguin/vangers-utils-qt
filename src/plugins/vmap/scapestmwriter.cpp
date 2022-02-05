#include "scapestmwriter.h"
#include "layer/heightimagelayer.h"
#include "layer/terrainimagelayer.h"
#include "binaryio.h"

#include <QFileInfo>
#include <QDir>
using namespace vangers;


void ScapeStmWriter::write(const Vmap& resource, const QString& filename)
{
	QSize size = resource.size();
	int32_t chunkSize = 2048 * size.width();

	int32_t maxTo = size.width() * size.height();
	QFileInfo fileInfo(filename);
	HeightImageLayer heightLayer;
	TerrainImageLayer terrainLayer;
	auto terrainImage = terrainLayer.getImage(resource, Level::Up);
	QSharedPointer<Matrix<uint8_t>> height = heightLayer.getHeight(resource, Level::Up);

	int32_t iChunk = 0;
	for(int32_t from = 0; from < maxTo; from += chunkSize){
		int32_t to = std::min(maxTo, from + chunkSize);
		QString stmFileName = fileInfo.absoluteDir().absoluteFilePath(fileInfo.baseName() + QString::number(from/chunkSize) + "." + fileInfo.suffix());
		QFile f(stmFileName);
		f.open(QFile::WriteOnly);
		writeChunk(*height, f, from, to);
		f.close();

		QString pngFileName = fileInfo.absoluteDir().absoluteFilePath(fileInfo.baseName() + QString::number(from/chunkSize) + ".png");
		QImage copy = terrainImage->copy(0, iChunk * 2048, 2048, (iChunk + 1) * 2048);
		copy.save(pngFileName);

		iChunk += 1;


	}
}

void ScapeStmWriter::writeChunk(const Matrix<uint8_t>& resource, QIODevice& device, int32_t from, int32_t to)
{
	BinaryWriter writer(&device);

	int32_t height = (to - from) / resource.sizeX();
	QTextStream stream(&device);
	stream << "STM "
			<< resource.sizeX() << " "
			<< height << " ";
	stream.flush();
	writer.write<int8_t>(4);
	writer.write<int8_t>(3);
	writer.write<int8_t>(2);
	writer.write<int8_t>(1);
	writer.write<int8_t>(0x0A);
	writer.write<int8_t>(0x0A);

	for(int32_t i = from; i < to; i++){
		writer.write<uint16_t>(resource[i]);
	}
}
