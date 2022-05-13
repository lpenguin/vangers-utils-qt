#ifndef C3DRENDERER_H
#define C3DRENDERER_H
#include <Qt3DRender/QGeometryRenderer>
#include <QBufferDataGenerator>
#include "model.h"

class C3DMesh : public Qt3DRender::QGeometryRenderer
{
	Q_OBJECT
public:
	C3DMesh(const QSharedPointer<model::C3D>& c3d, Qt3DCore::QNode *parent = nullptr);
private:

};

class C3DGeometry : public Qt3DRender::QGeometry {
public:
	C3DGeometry(const QSharedPointer<model::C3D>& c3d, QNode *parent);
private:
	void init();
	QSharedPointer<model::C3D> _c3d;
};

QByteArray createC3DVertexData(const model::C3D& c3d);
QByteArray createC3DIndexData(const model::C3D& c3d);

class C3DVertexBufferFunctor : public Qt3DRender::QBufferDataGenerator
{
public:
	explicit C3DVertexBufferFunctor(const QSharedPointer<model::C3D>& c3d)
		: _c3d(c3d)
	{}

	~C3DVertexBufferFunctor() {}

	QByteArray operator()() final
	{
		return createC3DVertexData(*_c3d);
	}

	bool operator ==(const Qt3DRender::QBufferDataGenerator &other) const final
	{
		const C3DVertexBufferFunctor *otherFunctor = functor_cast<C3DVertexBufferFunctor>(&other);
		if (otherFunctor != nullptr)
			return (otherFunctor->_c3d == _c3d);
		return false;
	}

	QT3D_FUNCTOR(C3DVertexBufferFunctor)
	private:
		QSharedPointer<model::C3D> _c3d;
};

class C3DIndexBufferFunctor : public Qt3DRender::QBufferDataGenerator
{
public:
	explicit C3DIndexBufferFunctor(const QSharedPointer<model::C3D>& c3d)
		: _c3d(c3d)
	{}

	~C3DIndexBufferFunctor() {}

	QByteArray operator()() final
	{
		return createC3DIndexData(*_c3d);
	}

	bool operator ==(const Qt3DRender::QBufferDataGenerator &other) const final
	{
		const C3DIndexBufferFunctor *otherFunctor = functor_cast<C3DIndexBufferFunctor>(&other);
		if (otherFunctor != nullptr)
			return (otherFunctor->_c3d == _c3d);
		return false;
	}

	QT3D_FUNCTOR(C3DVertexBufferFunctor)
	private:
		QSharedPointer<model::C3D> _c3d;
};
#endif // C3DRENDERER_H
