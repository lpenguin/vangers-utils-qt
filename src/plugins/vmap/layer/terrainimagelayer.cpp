#include "terrainimagelayer.h"
#include "../vmap.h"
#include "../vmapmeta.h"

TerrainImageLayer::TerrainImageLayer(QObject *parent)
	: AbstractImageLayer(parent)
{

}


QSharedPointer<QImage> TerrainImageLayer::getImage(const Vmap &vmap) const
{
	std::vector<uint8_t> meta = vmap.metaConst();

	uint8_t* data = new uint8_t[meta.size()];
	for(size_t i = 0; i < meta.size(); i++){
		uint8_t m = meta[i];
		uint8_t terrain = VmapMeta::fromMeta(m).terrain();
		data[i] = terrain;
	}

	vangers::Palette terrainPal;
	const vangers::Palette& pal = vmap.paletteConst();

	for(const std::pair<int, int> pair: vmap.terrainColorOffsetsConst()){
		int offset = pair.first;
		int offsetEnd = pair.second;
		terrainPal.append(pal[(offset + offsetEnd) / 2]);
	}

	return imageFromData(data, vmap.size().width(), vmap.size().height(), terrainPal);

}
