#ifndef TERRAINIMAGELAYER_H
#define TERRAINIMAGELAYER_H

#include "abstractimagelayer.h"

class TerrainImageLayer : public AbstractImageLayer
{
public:
	TerrainImageLayer(QObject* parent = nullptr);

	// AbstractImageLayer interface
public:
	QSharedPointer<QImage> getImage(const  Vmap& vmap, Level level) const;
};

#endif // TERRAINIMAGELAYER_H
