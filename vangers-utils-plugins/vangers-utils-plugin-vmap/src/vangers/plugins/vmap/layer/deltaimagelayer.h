#ifndef DELTAIMAGELAYER_H
#define DELTAIMAGELAYER_H

#include "abstractimagelayer.h"

namespace vangers::plugins::vmap {
	class DeltaImageLayer : public AbstractImageLayer
	{
	public:
		DeltaImageLayer(QObject* parent = nullptr);

		// AbstractImageLayer interface
	public:
		QSharedPointer<QImage> getImage(const Vmap &vmap, Level level) const;
	};
	}


#endif // DELTAIMAGELAYER_H
