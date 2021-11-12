#include "terrainimagelayer.h"
#include "../vmap.h"
#include "../vmapmeta.h"

TerrainImageLayer::TerrainImageLayer(QObject *parent)
	: AbstractImageLayer(parent)
{

}

QSharedPointer<QImage> fromData(uint8_t* data, int sizeX, int sizeY, const vangers::Palette& palette){
	QSharedPointer<QImage> image = QSharedPointer<QImage>::create(data, sizeX, sizeY, sizeX, QImage::Format_Indexed8);
	image->setColorTable(palette);
	return image;
}

QSharedPointer<QImage> TerrainImageLayer::getImage(const Vmap &vmap) const
{
	std::vector<uint8_t> meta = vmap.meta();

	uint8_t* data = new uint8_t[meta.size()];
	for(size_t i = 0; i < meta.size(); i++){
		uint8_t m = meta[i];
		uint8_t terrain = VmapMeta::fromMeta(m).terrain();
		data[i] = terrain;
	}

	vangers::Palette terrainPal;
	vangers::Palette pal = vmap.palette();

	for(std::pair<int, int> pair: vmap.terrainColorOffsets()){
		int offset = pair.first;
		int offsetEnd = pair.second;
		terrainPal.append(pal[(offset + offsetEnd) / 2]);
	}

	return fromData(data, vmap.sizeX(), vmap.sizeY(), terrainPal);

}
