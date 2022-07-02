#ifndef ISSHADOWEDIMAGELAYER_H
#define ISSHADOWEDIMAGELAYER_H

#include "abstractimagelayer.h"


namespace vangers::plugins::vmap {
	class IsShadowedImageLayer: public AbstractImageLayer
	{
	public:
		IsShadowedImageLayer(QObject *parent = nullptr);

		// AbstractImageLayer interface
	public:
		QSharedPointer<QImage> getImage(const Vmap &vmap, Level level) const;
	};

	}

#endif // ISSHADOWEDIMAGELAYER_H
