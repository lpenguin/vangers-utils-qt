#include "modelbinarywriterext.h"
#include <vangers/core/vector/vector.h>

using namespace m3d;
using namespace vangers::core::vector;
using namespace vangers::core::binario;

template<>
void BinaryWriter::write<Vector3I32>(const Vector3I32& vector)
{
	write(vector.x);
	write(vector.y);
	write(vector.z);
}

template<>
void BinaryWriter::write<Vector3F64>(const Vector3F64& vector)
{
	write(vector.x);
	write(vector.y);
	write(vector.z);
}

template<>
void BinaryWriter::write<Vector3F32>(const Vector3F32& vector)
{
	write(vector.x);
	write(vector.y);
	write(vector.z);
}

template<>
void BinaryWriter::write<Vector3I8>(const Vector3I8& vector)
{
	write(vector.x);
	write(vector.y);
	write(vector.z);
}


template<>
void BinaryWriter::write<Vertex>(const Vertex& vertex)
{
	write(vertex.tf);
	write(vertex.pos);
	write(vertex.sortInfo);
}

template<>
void BinaryWriter::write<Normal>(const Normal& normal)
{
	write(normal.normal);
	write(normal.nPower);
}

template<>
void BinaryWriter::write<PolygonIndex>(const PolygonIndex& index)
{
	write(index.vertInd);
	write(index.normInd);
}

template<>
void BinaryWriter::write<Polygon>(const Polygon& p)
{
	write(p.num);
	write(p.sortInfo);
	write(p.colorId);
	write(p.colorShift);
	write(p.flatNormal);
	write(p.middle);
	write(p.indices);
}

template<>
void BinaryWriter::write<C3D>(const C3D& c3d)
{
	write(c3d.version);
	write(c3d.numVert);
	write(c3d.numNorm);
	write(c3d.numPoly);
	write(c3d.numVertTotal);
	write(c3d.maxBound);
	write(c3d.minBound);
	write(c3d.offsets);
	write(c3d.rmax);
	write(c3d.angle);
	write(c3d.volume);
	write(c3d.rcm);
	write(c3d.J);
	write(c3d.vectices);
	write(c3d.normals);
	write(c3d.polygons);

	for(int i = 0; i < 3; i++){
		write(c3d.polyInds[i]);
	}
}

template<>
void BinaryWriter::write<Wheel>(const Wheel& p)
{
	write(p.steer);
	write(p.f);
	write(p.width);
	write(p.radius);
	write(p.boundIndex);
	write(p.model);
}

template<>
void BinaryWriter::write<Slot>(const Slot& p)
{
	write(p.rSlots);
	write(p.locationAngleOfSlots);
}

template<>
void BinaryWriter::write<M3D>(const M3D& m3d)
{
	write(m3d.body);
	write(m3d.bounds);
	write(m3d.rMax);
	write(m3d.nWheels);
	write(m3d.nDebris);
	write(m3d.bodyColorOffset);
	write(m3d.bodyColorShift);
	write(m3d.wheels);
	for(int i = 0; i < m3d.nDebris; i++){
		write(m3d.debris[i]);
		write(m3d.boundDebris[i]);
	}
	write(m3d.bound);
	write(m3d.slotsExistence);
	if(m3d.slotsExistence != 0){
		write(m3d.rSlots);
	}
}
