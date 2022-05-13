#include "modelbinarywriterext.h"

template<>
void vangers::BinaryWriter::write<model::Vector3I32>(const model::Vector3I32& vector)
{
	write(vector.x);
	write(vector.y);
	write(vector.z);
}

template<>
void vangers::BinaryWriter::write<model::Vector3F64>(const model::Vector3F64& vector)
{
	write(vector.x);
	write(vector.y);
	write(vector.z);
}

template<>
void vangers::BinaryWriter::write<model::Vector3F32>(const model::Vector3F32& vector)
{
	write(vector.x);
	write(vector.y);
	write(vector.z);
}

template<>
void vangers::BinaryWriter::write<model::Vector3I8>(const model::Vector3I8& vector)
{
	write(vector.x);
	write(vector.y);
	write(vector.z);
}


template<>
void vangers::BinaryWriter::write<model::Vertex>(const model::Vertex& vertex)
{
	write(vertex.tf);
	write(vertex.pos);
	write(vertex.sortInfo);
}

template<>
void vangers::BinaryWriter::write<model::Normal>(const model::Normal& normal)
{
	write(normal.normal);
	write(normal.nPower);
}

template<>
void vangers::BinaryWriter::write<model::PolygonIndex>(const model::PolygonIndex& index)
{
	write(index.vertInd);
	write(index.normInd);
}

template<>
void vangers::BinaryWriter::write<model::Polygon>(const model::Polygon& p)
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
void vangers::BinaryWriter::write<model::C3D>(const model::C3D& c3d)
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
	write(c3d.vectices);
	write(c3d.normals);
	write(c3d.polygons);
	for(int i = 0; i < 3; i++){
		write(c3d.polyInds[i]);
	}
}
