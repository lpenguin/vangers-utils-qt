#ifndef HEIGHTIMAGELAYER_H
#define HEIGHTIMAGELAYER_H

#include "abstractimagelayer.h"

class HeightImageLayer : public AbstractImageLayer
{
public:
	HeightImageLayer(QObject* parent = nullptr);

	// AbstractImageLayer interface
public:
	QSharedPointer<QImage> getImage(const Vmap &vmap, Level level) const;
};

#endif // HEIGHTIMAGELAYER_H
