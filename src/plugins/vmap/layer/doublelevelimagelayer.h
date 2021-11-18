#ifndef DOUBLELEVELIMAGELAYER_H
#define DOUBLELEVELIMAGELAYER_H

#include "abstractimagelayer.h"



class DoubleLevelImageLayer: public AbstractImageLayer
{
public:
	DoubleLevelImageLayer(QObject *parent = nullptr)
		:AbstractImageLayer(parent)
	{};

	// AbstractImageLayer interface
public:
	QSharedPointer<QImage> getImage(const Vmap &vmap, Level level) const;
};

#endif // DOUBLELEVELIMAGELAYER_H
