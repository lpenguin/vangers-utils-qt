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
	void fillData(const Vmap &vmap, Level level, std::vector<uint8_t>& data) const;
};

#endif // HEIGHTIMAGELAYER_H
