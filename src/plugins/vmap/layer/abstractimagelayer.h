#ifndef ABSTRACTIMAGELAYER_H
#define ABSTRACTIMAGELAYER_H

#include <QObject>
#include <QSharedPointer>
#include <QImage>
#include "../vmap.h"

class AbstractImageLayer : public QObject
{
public:
	explicit AbstractImageLayer(QObject *parent = nullptr);
	virtual QSharedPointer<QImage> getImage(const Vmap& vmap) const = 0;
protected:
	virtual ~AbstractImageLayer() {};
signals:

};

#endif // ABSTRACTIMAGELAYER_H
