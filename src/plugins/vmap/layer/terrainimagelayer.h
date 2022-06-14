#ifndef TERRAINIMAGELAYER_H
#define TERRAINIMAGELAYER_H

#include "abstractimagelayer.h"
#include <image/palette.h>

class TerrainImageLayer : public AbstractImageLayer
{
public:
	TerrainImageLayer(QObject* parent = nullptr);

	// AbstractImageLayer interface
public:
	QSharedPointer<QImage> getImage(const  Vmap& vmap, Level level) const;
	void fillData(const Vmap &vmap, Level level, std::vector<uint8_t>& data) const;
	vangers::Palette getPalette(const Vmap &vmap) const;

};

#endif // TERRAINIMAGELAYER_H
