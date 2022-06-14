#ifndef ALLIMAGELAYER_H
#define ALLIMAGELAYER_H

#include "abstractimagelayer.h"


namespace vangers::plugins::vmap {
	class AllImageLayer : public AbstractImageLayer
	{
	public:
		AllImageLayer(QObject *parent = nullptr);

		// AbstractImageLayer interface
	public:
		QSharedPointer<QImage> getImage(const Vmap &vmap, Level level) const;
	};
	}


#endif // ALLIMAGELAYER_H
