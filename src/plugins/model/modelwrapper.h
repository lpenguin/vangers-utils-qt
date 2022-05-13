#ifndef M3DWRAPPER_H
#define M3DWRAPPER_H

#include <QObject>
#include <QVector3D>
#include "model.h"

class C3DWrapper;

class M3DWrapper : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QVector3D bounds READ bounds)
public:
	explicit M3DWrapper(model::M3D m3d, QObject *parent = nullptr);

	QVector3D bounds() const;

	int32_t getRMax() const;

	model::M3D getM3d() const;

signals:
private:
	model::M3D _m3d;
};

class C3DWrapper : public QObject {
	Q_OBJECT
	Q_PROPERTY(QVector3D maxBound READ maxBound)
	Q_PROPERTY(QVector3D minBound READ minBound)
public:
	QVector3D maxBound() const;

	QVector3D minBound() const;

private:
	model::C3D _c3d;
};

#endif // M3DWRAPPER_H
