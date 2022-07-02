#include "terrainimagelayer.h"
#include "../vmap.h"
#include "../vmapmeta.h"

using namespace vangers::plugins::vmap;
using namespace vangers::core::palette;

TerrainImageLayer::TerrainImageLayer(QObject *parent)
	: AbstractImageLayer(parent)
{

}


QSharedPointer<QImage> TerrainImageLayer::getImage(const Vmap &vmap, Level level) const
{
	std::vector<uint8_t> meta = vmap.metaConst();

	uint8_t* data = new uint8_t[meta.size()];
	for(size_t i = 0; i < meta.size(); i++){
		uint8_t m = meta[i];
		VmapMeta currentMeta = VmapMeta::fromMeta(m);
		uint32_t metaIndex = i;
		if(currentMeta.isDoubleLevel() && level != Level::Both){
			if(level == Level::Up){
				metaIndex = metaIndex | 1;
			} else if(level == Level::Down){
				metaIndex = metaIndex & ~1;
			}
		}

		if(metaIndex != i){
			currentMeta = VmapMeta::fromMeta(meta[metaIndex]);
		}
		data[i] = currentMeta.terrain();
	}

	Palette terrainPal;
	const Palette& pal = vmap.paletteConst();

	for(const std::pair<int, int> pair: vmap.terrainColorOffsetsConst()){
		int offset = pair.first;
		int offsetEnd = pair.second;
		terrainPal.append(pal[(offset + offsetEnd) / 2]);
	}

	return imageFromData(data, vmap.size().width(), vmap.size().height(), terrainPal);

}


void TerrainImageLayer::fillData(const Vmap &vmap, Level level, std::vector<uint8_t>& data) const
{
	const std::vector<uint8_t>& meta = vmap.metaConst();
	const std::vector<uint8_t>& height = vmap.heightConst();

	data.resize(meta.size());
	for(size_t i = 0; i < meta.size(); i++){
		uint8_t m = meta[i];
		VmapMeta currentMeta = VmapMeta::fromMeta(m);
		uint32_t metaIndex = i;
		if(currentMeta.isDoubleLevel() && level != Level::Both){
			if(level == Level::Up){
				metaIndex = metaIndex | 1;
			} else if(level == Level::Down){
				metaIndex = metaIndex & ~1;
			}
		}

		if(metaIndex != i){
			currentMeta = VmapMeta::fromMeta(meta[metaIndex]);
		}
		data[i] = currentMeta.terrain();
	}
}

vangers::core::palette::Palette TerrainImageLayer::getPalette(const Vmap &vmap) const
{
	vangers::core::palette::Palette terrainPal;
	const vangers::core::palette::Palette& pal = vmap.paletteConst();

	for(const std::pair<int, int>& pair: vmap.terrainColorOffsetsConst()){
		int offset = pair.first;
		int offsetEnd = pair.second;
		terrainPal.append(pal[(offset + offsetEnd) / 2]);
	}
	return terrainPal;
}
