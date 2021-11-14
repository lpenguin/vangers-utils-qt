#include "vmapwriter.h"

#include <QDir>
#include <QFileInfo>
#include <QSharedPointer>
#include <QImage>
#include <QSettings>

#include <plugins/palette/pngpaletteaccess.h>


void vangers::VmapWriter::write(const Vmap &vmap, QString filename)
{
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

	auto height = imageFromData(vmap.heightConst().data(), sizeX, sizeY, palette);
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
