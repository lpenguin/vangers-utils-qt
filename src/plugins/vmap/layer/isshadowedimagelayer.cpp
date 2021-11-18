#include "isshadowedimagelayer.h"

#include <plugins/vmap/vmapmeta.h>

IsShadowedImageLayer::IsShadowedImageLayer(QObject *parent)
	:AbstractImageLayer(parent)
{

}

QSharedPointer<QImage> IsShadowedImageLayer::getImage(const Vmap &vmap, Level level) const
{
	const Matrix<uint8_t>& meta = vmap.metaConst();

	vangers::Palette pal;
	pal		<<qRgb(0, 0, 0)
			<< qRgb(255, 255, 255);
	uint8_t* data = new uint8_t[meta.size()];
	for(size_t i =0; i < meta.size(); i++){
		VmapMeta metaInfo = VmapMeta::fromMeta(meta[i]);
		data[i] = metaInfo.isShadowed() ? 1U : 0U;
	}
	return imageFromData(data, vmap.size().width(), vmap.size().height(), pal);
}
