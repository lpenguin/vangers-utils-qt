#include "heightimagelayer.h"

#include <plugins/vmap/vmapmeta.h>

HeightImageLayer::HeightImageLayer(QObject *parent)
	:AbstractImageLayer(parent)
{

}

QSharedPointer<QImage> HeightImageLayer::getImage(const Vmap &vmap, Level level) const
{
	std::vector<uint8_t> meta = vmap.metaConst();
	std::vector<uint8_t> height = vmap.heightConst();

	uint8_t* data = new uint8_t[meta.size()];
	for(size_t i = 0; i < meta.size(); i++){
		uint8_t m = meta[i];
		VmapMeta currentMeta = VmapMeta::fromMeta(m);
		uint32_t heightIndex = i;
		if(currentMeta.isDoubleLevel() && level != Level::Both){
			if(level == Level::Up){
				heightIndex = heightIndex | 1;
			} else if(level == Level::Down){
				heightIndex = heightIndex & ~1;
			}
		}

		data[i] = height[heightIndex];
	}

	vangers::Palette pal = vangers::Palette::grayscale();


	return imageFromData(data, vmap.size().width(), vmap.size().height(), pal);
}
