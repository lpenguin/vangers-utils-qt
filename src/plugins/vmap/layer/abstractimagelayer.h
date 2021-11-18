#ifndef ABSTRACTIMAGELAYER_H
#define ABSTRACTIMAGELAYER_H

#include <QObject>
#include <QSharedPointer>
#include <QImage>
#include "../vmap.h"

enum class Level {
	Both = 0,
	Up = 1,
	Down = 2,
	MAX_LEVEL,
	Invalid,
};

class AbstractImageLayer : public QObject
{
public:
	explicit AbstractImageLayer(QObject *parent = nullptr);
	virtual QSharedPointer<QImage> getImage(const Vmap& vmap, Level level) const = 0;
protected:
	virtual ~AbstractImageLayer() {};
signals:

};

#endif // ABSTRACTIMAGELAYER_H
