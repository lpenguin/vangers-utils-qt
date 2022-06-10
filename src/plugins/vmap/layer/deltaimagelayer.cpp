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
		int32_t evenX, oddX;
		if(i % 2 == 0) {
			evenX = i;
			oddX = i + 1;
		} else {
			evenX = i - 1;
			oddX = i;
		}

		int32_t evenDelta = VmapMeta::fromMeta(meta[evenX]).delta();
		int32_t oddDelta = VmapMeta::fromMeta(meta[oddX]).delta();

//		int32_t evenDelta = VmapMeta::fromMeta(meta[i & !1]).delta();
//		int32_t oddDelta = VmapMeta::fromMeta(meta[i | 1]).delta();
		int32_t delta = (evenDelta << 2) + oddDelta;
		data[i] = delta;
	}

	int32_t deltaMax = 1 << 4;
	for(int i = 0; i < deltaMax; i++){
		int32_t c = i * (255.0 / deltaMax);
		pal << qRgb(c, c, c);
	}

	return imageFromData(data, vmap.size().width(), vmap.size().height(), pal);
}
