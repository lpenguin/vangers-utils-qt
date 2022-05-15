#include "modelbinaryreaderext.h"
#include <core/vector.h>

using namespace vangers::core::vector;
using namespace vangers::model::m3d;

template<typename T>
bool readVector(Vector3<T>& vector, vangers::BinaryReader &reader){
	return reader.tryRead(vector.x) && reader.tryRead(vector.y) && reader.tryRead(vector.z);
}

template<typename T>
bool readMatrix(Matrix3x3<T>& matrix, vangers::BinaryReader &reader){
	if(!reader.tryRead(matrix.values)){
		return false;
	}

	return true;
}

template<>
bool vangers::BinaryReader::tryRead<Matrix3x3F64>(Matrix3x3F64& vector){
	return readMatrix(vector, *this);
}

template<>
bool vangers::BinaryReader::tryRead<Vector3F32>(Vector3F32& vector){
	return readVector(vector, *this);
}

template<>
bool vangers::BinaryReader::tryRead<Vector3F64>(Vector3F64& vector){
	return readVector(vector, *this);
}

template<>
bool vangers::BinaryReader::tryRead<Vector3I8>(Vector3I8& vector){
	return readVector(vector, *this);
}

template<>
bool vangers::BinaryReader::tryRead<Vector3I32>(Vector3I32& vector){
	return readVector(vector, *this);
}

template<>
bool vangers::BinaryReader::tryRead<Vertex>(Vertex &vertex)
{
	if(!tryRead(vertex.tf)){
		return false;
	}

	if(!tryRead(vertex.pos)){
		return false;
	}

	if(!tryRead(vertex.sortInfo)){
		return false;
	}

	return true;
}

template<>
bool vangers::BinaryReader::tryRead<Normal>(Normal &normal)
{
	if(!tryRead(normal.normal)){
		return false;
	}

	if(!tryRead(normal.nPower)){
		return false;
	}

	return true;
}

template<>
bool vangers::BinaryReader::tryRead<Polygon>(Polygon &polygon)
{
	if(!tryRead(polygon.num)){
		return false;
	}

	if(!tryRead(polygon.sortInfo)){
		return false;
	}

	if(!tryRead(polygon.colorId)){
		return false;
	}

	if(!tryRead(polygon.colorShift)){
		return false;
	}

	if(!tryRead(polygon.flatNormal)){
		return false;
	}

	if(!tryRead(polygon.middle)){
		return false;
	}

	if(!tryRead(polygon.indices, polygon.num)){
		return false;
	}

	return true;
}

template<>
bool vangers::BinaryReader::tryRead<PolygonIndex>(PolygonIndex &polygonIndex)
{
	if(!tryRead(polygonIndex.vertInd)){
		return false;
	}

	if(!tryRead(polygonIndex.normInd)){
		return false;
	}

	return true;
}


template<>
bool vangers::BinaryReader::tryRead<C3D>(C3D &c3d)
{
	if(!tryRead(c3d.version)){
		return false;
	}

	if(c3d.version != 8){
		return false;
	}

	if(!tryRead(c3d.numVert)){
		return false;
	}

	if(!tryRead(c3d.numNorm)){
		return false;
	}

	if(!tryRead(c3d.numPoly)){
		return false;
	}

	if(!tryRead(c3d.numVertTotal)){
		return false;
	}

	if(!tryRead(c3d.maxBound)){
		return false;
	}

	if(!tryRead(c3d.minBound)){
		return false;
	}

	if(!tryRead(c3d.offsets)){
		return false;
	}

	if(!tryRead(c3d.rmax)){
		return false;
	}

	if(!tryRead(c3d.angle)){
		return false;
	}

	if(!(c3d.angle.x == 83)){
		return false;
	}

	if(!tryRead(c3d.volume)){
		return false;
	}

	if(!tryRead(c3d.rcm)){
		return false;
	}

	if(!tryRead(c3d.J)){
		return false;
	}

	if(!tryRead(c3d.vectices, c3d.numVert)){
		return false;
	}

	if(!tryRead(c3d.normals, c3d.numNorm)){
		return false;
	}

	if(!tryRead(c3d.polygons, c3d.numPoly)){
		return false;
	}

	for(int i = 0; i < 3; i++){
		if(!tryRead(c3d.polyInds[i], c3d.numPoly)){
			return false;
		}
	}

	return true;
}

template<>
bool vangers::BinaryReader::tryRead<Wheel>(Wheel &wheel){
	if(!tryRead(wheel.steer)){
		return false;
	}

	if(!tryRead(wheel.f)){
		return false;
	}

	if(!tryRead(wheel.width)){
		return false;
	}

	if(!tryRead(wheel.radius)){
		return false;
	}

	if(!tryRead(wheel.boundIndex)){
		return false;
	}

	if(!tryRead(wheel.model, wheel.steer)){
		return false;
	}

	return true;
}

template<>
bool vangers::BinaryReader::tryRead<Slot>(Slot &slot){
	if(!tryRead(slot.rSlots)){
		return false;
	}

	if(!tryRead(slot.locationAngleOfSlots)){
		return false;
	}

	return true;
}


template<>
bool vangers::BinaryReader::tryRead<M3D>(M3D &m3d){
	if(!tryRead(m3d.body)){
		return false;
	}

	if(!tryRead(m3d.bounds)){
		return false;
	}


	if(!tryRead(m3d.rMax)){
		return false;
	}

	if(!tryRead(m3d.nWheels)){
		return false;
	}

	if(!tryRead(m3d.nDebris)){
		return false;
	}

	if(!tryRead(m3d.bodyColorOffset)) return false;

	if(!tryRead(m3d.bodyColorShift)) return false;

	if(!tryRead(m3d.wheels, m3d.nWheels)) return false;

//	if(!tryRead(m3d.debris, m3d.nDebris)) return false;
//	if(!tryRead(m3d.boundDebris, m3d.nDebris)) return false;

	m3d.debris.resize(m3d.nDebris);
	m3d.boundDebris.resize(m3d.nDebris);

	for(int i = 0; i < m3d.nDebris; i++){
		if(!tryRead(m3d.debris[i])) return false;
		if(!tryRead(m3d.boundDebris[i])) return false;
	}

	if(!tryRead(m3d.bound)) return false;

	if(!tryRead(m3d.slotsExistence)) return false;

	if(m3d.slotsExistence != 0){
		if(!tryRead(m3d.rSlots, 3)) return false;
	}
	return true;
}

template<>
bool vangers::BinaryReader::tryRead<A3D>(A3D& a3d)
{
	if(!tryRead(a3d.nModels)) return false;

	if(!tryRead(a3d.bounds)) return false;

	if(!tryRead(a3d.rMax)) return false;

	if(!tryRead(a3d.bodyColorOffset)) return false;

	if(!tryRead(a3d.bodyColorShift)) return false;

	if(!tryRead(a3d.models, a3d.nModels)) return false;

	return true;
}
