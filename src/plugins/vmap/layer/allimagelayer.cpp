#include "allimagelayer.h"

AllImageLayer::AllImageLayer(QObject *parent)
	: AbstractImageLayer(parent)
{

}

QSharedPointer<QImage> AllImageLayer::getImage(const Vmap &vmap, Level level) const
{
	return imageFromData(vmap.metaConst().data(), vmap.size().width(), vmap.size().height(), vmap.paletteConst());
}
