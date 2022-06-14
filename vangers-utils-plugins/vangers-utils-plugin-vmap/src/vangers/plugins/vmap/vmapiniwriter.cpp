#include "vmapiniwriter.h"

#include <QDir>
#include <QFileInfo>
#include <QSharedPointer>
#include <QImage>
#include <QSettings>
#include "layer/heightimagelayer.h"
#include "layer/terrainimagelayer.h"

#include <vangers/plugins/palette/pngpaletteaccess.h>

using namespace vangers::core::palette;
using namespace vangers::plugins::vmap;
using namespace vangers::plugins::palette;

void VmapIniWriter::write(const Vmap &vmap, QString filename, Level level)
{
	const uint8_t selectedTerrainIndex = 1;


	QSettings settings(filename, QSettings::IniFormat);
	QDir baseDir(QFileInfo(filename).absoluteDir());
	QString heightFilename = baseDir.filePath("height.png");
	QString metaFilename = baseDir.filePath("meta.png");
	QString paletteFilename = baseDir.filePath("palette.png");
	QString tableFilename = baseDir.filePath("table.png");

	settings.setValue("height", heightFilename);
	settings.setValue("meta", metaFilename);
	settings.setValue("palette", paletteFilename);
	settings.setValue("table", tableFilename);
	settings.sync();

	int sizeX = vmap.size().width();
	int sizeY = vmap.size().height();

	Palette palette = Palette::grayscale();

//	TerrainImageLayer terrainLayer = new TerrainImageLayer();
//	HeightImageLayer heightLayer = new HeightImageLayer();
	Matrix<uint8_t> terrainData(sizeX, sizeY);
	Matrix<uint8_t> heightData(sizeX, sizeY);
//	terrainLayer.fillData(vmap, level, terrainData);
//	heightLayer.fillData(vmap, level, heightData);

	for(int iY = 0; iY < sizeY; iY++){
		uint8_t prevHeight = 0;

		for(int iX = 0; iX < sizeX; iX++){
			uint8_t terrainIndex = terrainData.getData(iX, iY);
			uint8_t height;
			if(terrainIndex == selectedTerrainIndex){
				height = heightData.getData(iX, iY);
				prevHeight = height;
			} else {
				height = 0;
			}
			heightData.setData(iX, iY, height);
		}
	}

	auto height = imageFromDataRed(heightData.data(), sizeX, sizeY);
	auto meta = imageFromData(vmap.metaConst().data(), sizeX, sizeY, palette);

	height->save(heightFilename);
	meta->save(metaFilename);

	const auto& offsets = vmap.terrainColorOffsetsConst();

	size_t offsetSize = offsets.size();
	PngPaletteAccess pngWriter;
	pngWriter.writeToFile(vmap.paletteConst(), paletteFilename);


	uint8_t* table = new uint8_t[offsetSize* 4];
	for(size_t i = 0; i < offsetSize; i ++ ){
		table[i * 4 + 0] = (uint8_t)offsets[i].first;
		table[i * 4 + 1] = (uint8_t)offsets[i].second;
		table[i * 4 + 2] = 0;
		table[i * 4 + 3] = 0;
	}

	QImage image(table, offsetSize, 1, offsetSize * 4, QImage::Format_RGBA8888);
	image.save(tableFilename);
	delete[] table;
}
