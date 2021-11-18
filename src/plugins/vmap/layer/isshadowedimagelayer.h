#ifndef ISSHADOWEDIMAGELAYER_H
#define ISSHADOWEDIMAGELAYER_H

#include "abstractimagelayer.h"



class IsShadowedImageLayer: public AbstractImageLayer
{
public:
	IsShadowedImageLayer(QObject *parent = nullptr);

	// AbstractImageLayer interface
public:
	QSharedPointer<QImage> getImage(const Vmap &vmap, Level level) const;
};

#endif // ISSHADOWEDIMAGELAYER_H
