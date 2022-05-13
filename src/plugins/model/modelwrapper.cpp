#include "modelwrapper.h"

template<typename T>
QVector3D toVec3(const T& vec){
	return QVector3D(vec.x, vec.y, vec.z);
}
M3DWrapper::M3DWrapper(model::M3D m3d, QObject *parent)
	: QObject(parent)
	, _m3d(m3d)
{

}

QVector3D M3DWrapper::bounds() const
{
	return toVec3(_m3d.bounds);
}

int32_t M3DWrapper::getRMax() const
{
	return _m3d.rMax;
}

model::M3D M3DWrapper::getM3d() const
{
	return _m3d;
}


QVector3D C3DWrapper::maxBound() const
{
	return toVec3(_c3d.maxBound);
}

QVector3D C3DWrapper::minBound() const
{
	return toVec3(_c3d.minBound);
}
