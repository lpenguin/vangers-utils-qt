#ifndef FLOODLAYER_H
#define FLOODLAYER_H
#include "abstractimagelayer.h"

namespace vangers::plugins::vmap {
	class FloodLayer: public AbstractImageLayer
	{
	public:
		FloodLayer(QObject* parent = nullptr);

	public:
		QSharedPointer<QImage> getImage(const Vmap &vmap, Level level) const;
	};

	}

#endif // FLOODLAYER_H
