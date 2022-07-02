#ifndef DOUBLELEVELIMAGELAYER_H
#define DOUBLELEVELIMAGELAYER_H

#include "abstractimagelayer.h"


namespace vangers::plugins::vmap {
	class DoubleLevelImageLayer: public AbstractImageLayer
	{
	public:
		DoubleLevelImageLayer(QObject *parent = nullptr)
			:AbstractImageLayer(parent)
		{};

		// AbstractImageLayer interface
	public:
		QSharedPointer<QImage> getImage(const Vmap &vmap, Level level) const;
		void fillData(const Vmap &vmap, Level level, std::vector<uint8_t>& data) const;
	};
	}


#endif // DOUBLELEVELIMAGELAYER_H
