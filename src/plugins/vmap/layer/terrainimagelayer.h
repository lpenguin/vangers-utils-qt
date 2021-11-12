#ifndef TERRAINIMAGELAYER_H
#define TERRAINIMAGELAYER_H

#include "abstractimagelayer.h"

class TerrainImageLayer : public AbstractImageLayer
{
public:
	TerrainImageLayer(QObject* parent);

	// AbstractImageLayer interface
public:
	QSharedPointer<QImage> getImage(const  Vmap& vmap) const;
};

#endif // TERRAINIMAGELAYER_H
