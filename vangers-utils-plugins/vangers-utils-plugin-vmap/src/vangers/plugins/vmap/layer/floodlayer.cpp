#include "floodlayer.h"
#include <vangers/core/palette/palette.h>


using namespace vangers::plugins::vmap;
using namespace vangers::core::palette;

FloodLayer::FloodLayer(QObject* parent)
	: AbstractImageLayer(parent)
{

}

QSharedPointer<QImage> FloodLayer::getImage(const Vmap& vmap, Level level) const
{
	const auto& size = vmap.size();
	const auto& height = vmap.heightConst();
	const auto& flood = vmap.floodConst();

	uint8_t* data = new uint8_t[height.size()];
	auto pal = Palette::grayscale();
	int chunkSize = size.height() / flood.size();

	for(int iFlood = 0; iFlood < flood.size(); iFlood++){
		uint32_t floodValue = flood[iFlood];
		for(int iRow = iFlood * chunkSize; iRow < (iFlood + 1) * chunkSize; iRow++){
			for(int iCol = 0; iCol < vmap.size().width(); iCol++){
				data[iRow * size.width() + iCol] = (uint8_t)floodValue;
			}
		}
	}

	return imageFromData(data, size.width(), size.height(), pal);
}
