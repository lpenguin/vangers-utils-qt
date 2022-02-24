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
		std::vector<int32_t> polyInds[3];
	};

	struct Wheel{
		int32_t steer;
		Vector3F64 f;
		int32_t width;
		int32_t radius;
		int32_t boundIndex;
		std::vector<C3D> model;
	};

	struct Slot {
		Vector3I32 rSlots;
		int32_t locationAngleOfSlots;
	};

	struct M3D {
		C3D body;
		Vector3I32 bounds;
		int32_t rMax;
		int32_t nWheels;
		int32_t nDebris;
		int32_t bodyColorOffset;
		int32_t bodyColorShift;
		std::vector<Wheel> wheels;
		std::vector<C3D> debris;
		std::vector<C3D> boundDebris;
		C3D bound;
		int32_t slotsExistence;
		std::vector<Slot> rSlots;
	};

	struct A3D {
		int32_t nModels;
		Vector3I32 bounds;
		int32_t rMax;
		int32_t bodyColorOffset;
		int32_t bodyColorShift;
		std::vector<C3D> models;
	};
}

#endif // MODEL_H


