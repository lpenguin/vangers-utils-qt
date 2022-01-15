#ifndef MODEL_H
#define MODEL_H
#include <inttypes.h>
#include <utility>
#include <vector>

namespace model {
	template<typename T>
	struct Vector3 {
		T x;
		T y;
		T z;
	};

	template<typename T>
	struct Matrix3x3 {
		T values[9];
	};

	typedef Vector3<int8_t> Vector3I8;
	typedef Vector3<int32_t> Vector3I32;
	typedef Vector3<double> Vector3F64;
	typedef Vector3<float> Vector3F32;
	typedef Matrix3x3<double> Matrix3x3F64;

	struct Vertex {
		Vector3F32 tf;
		Vector3I8 pos;
		int32_t sortInfo;
	};

	struct Normal {
		Vector3I8 normal;
		uint8_t nPower;
	};

	struct PolygonNormal: Normal {
		int32_t sortInfo;
	};

	struct PolygonIndex {
		int32_t vertInd;
		int32_t normInd;
	};

	struct Polygon {
		int32_t num;
		int32_t sortInfo;
		uint32_t colorId;
		uint32_t colorShift;
		Normal flatNormal;
		Vector3I8 middle;
		std::vector<PolygonIndex> indices;
	};

	struct C3D {
		int32_t version;
		int32_t numVert;
		int32_t numNorm;
		int32_t numPoly;
		int32_t numVertTotal;
		Vector3I32 maxBound;
		Vector3I32 minBound;
		Vector3I32 offsets;
		int32_t rmax;
		Vector3I32 angle;
		double volume;
		Vector3F64 rcm;
		Matrix3x3F64 J;
		std::vector<Vertex> vectices;
		std::vector<PolygonNormal> normals;
		std::vector<Polygon> polygons;
		std::vector<PolygonIndex> polyInds[3];
	};

	struct M3D {
		C3D body;
//		Vector3I32 bounds;
//		int32_t rMax;
//		int32_t nWheels;
//		int32_t rMax;
	};

}

#endif // MODEL_H


