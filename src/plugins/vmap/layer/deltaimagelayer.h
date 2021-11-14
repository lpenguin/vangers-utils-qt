#ifndef DELTAIMAGELAYER_H
#define DELTAIMAGELAYER_H

#include "abstractimagelayer.h"

class DeltaImageLayer : public AbstractImageLayer
{
public:
	DeltaImageLayer(QObject* parent = nullptr);

	// AbstractImageLayer interface
public:
	QSharedPointer<QImage> getImage(const Vmap &vmap) const;
};

#endif // DELTAIMAGELAYER_H
