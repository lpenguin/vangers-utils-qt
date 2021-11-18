#include "deltaimagelayer.h"

#include <plugins/vmap/vmapmeta.h>

DeltaImageLayer::DeltaImageLayer(QObject *parent)
	:AbstractImageLayer(parent)
{

}

QSharedPointer<QImage> DeltaImageLayer::getImage(const Vmap &vmap, Level level) const
{
	const Matrix<uint8_t>& meta = vmap.metaConst();

	vangers::Palette pal;

	uint8_t* data = new uint8_t[meta.size()];
	for(size_t i =0; i < meta.size(); i++){
		VmapMeta metaInfo = VmapMeta::fromMeta(meta[i]);
		data[i] = metaInfo.delta();
	}

	pal << qRgb(0xB6, 0xB2, 0x90)
		<< qRgb(0xDD, 0xDB, 0xCB)
		<< qRgb(0xF5, 0xF1, 0xE3)
		<< qRgb(0xFF, 0xFF, 0xFF);

	return imageFromData(data, vmap.size().width(), vmap.size().height(), pal);
}
