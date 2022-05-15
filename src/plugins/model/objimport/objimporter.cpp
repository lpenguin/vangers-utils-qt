#include "objimporter.h"
#include <QSet>
#include <volint/volint.h>
#include <QDebug>

using namespace vangers::model::objimport;

struct Bounds;

void makeBoundingBox(Object& result, const Bounds& bounds);
void skinBodyBounds(const Object& body, Object& bounds);
void alterBoundingBox(C3D& bounds, C3D& body, bool isMechous);
void calcPhysics(C3D& c3d);
QList<Face> triangulate(const Face& face){
	if(face.indices.size() == 3){
		return {face};
	}

	QList<Face> result;
	FaceIndex i0 = face.indices[0];
	for(int i = 2; i< face.indices.size(); i++){
		const FaceIndex& iCurr = face.indices[i];
		const FaceIndex& iPrev = face.indices[i - 1];
		result.append({{i0, iPrev, iCurr}});
	}

	return result;
}


struct Bounds {
	Vector3F64 min;
	Vector3F64 max;

	Vector3F64 origin() const {
		return (min + max) / 2.0;
	}
};



void calcPhysics(C3D& c3d){
	double(* verts)[3] = new double[c3d.vectices.size()][3];

	for(int i = 0; i < c3d.vectices.size(); i++){
		const Vertex& v = c3d.vectices[i];
		verts[i][0] = v.tf.x;
		verts[i][1] = v.tf.y;
		verts[i][2] = v.tf.z;
	}

	volint::Polyhedron polyhedron {
	};

	int nVertices = c3d.vectices.size();
	int nFaces = 0;

	volint::PolyhedronFace* faces = new volint::PolyhedronFace[c3d.polygons.size()];
	for(int i = 0; i < c3d.polygons.size(); i++){
		const Polygon& p = c3d.polygons[i];
		const Vector3I8& n = p.flatNormal.normal;
		if(n.x == 0 && n.y == 0 && n.z == 0){
			continue;
		}
		volint::PolyhedronFace& f = faces[nFaces++];
		f.poly = &polyhedron;
		f.numVerts = p.indices.size();
		f.verts = new int[p.indices.size()];
		for(int j = 0; j < p.indices.size(); j++){
			const PolygonIndex& index = p.indices[j];
			if(index.vertInd < 0 || index.vertInd >= c3d.vectices.size()){
				return;
			}
			f.verts[p.indices.size() - 1 - j] = index.vertInd;
		}
	}
	polyhedron.numFaces = nFaces;
	polyhedron.numVerts = nVertices;
	polyhedron.verts = verts;
	polyhedron.faces = faces;

	double J[3][3];
	double r[3];
	double volume;

	volint::VolumeIntegral vint;
	vint.compute(polyhedron, J, r, volume);
	c3d.volume = volume;
	c3d.J = {
		J[0][0], J[0][1], J[0][2],
		J[1][0], J[1][1], J[1][2],
		J[2][0], J[2][1], J[2][2],
	};
	c3d.rcm = {r[0], r[1], r[2]};
}

void objectToC3D(
		const Object& obj,
		C3D& c3d,
		const ObjImportSettings& importSettings,
		double scaleFactor,
		Bounds bounds){

	std::vector<Vertex> vertices;
	std::vector<PolygonNormal> normals;
	std::vector<Polygon> polygons;

	Vector3F64 origin = (bounds.max + bounds.min) / 2.0;

	for(const Vector3F64& vertex: obj.vertices){
		Vector3F64 objVertexScaled = (vertex  - origin) * scaleFactor;

		Vertex v {
			.tf = objVertexScaled,
			.pos = objVertexScaled,
			.sortInfo = 0,
		};
		vertices.push_back(v);
	}

	for(const Vector3F64& normal: obj.normals) {
		Vector3F64 normalScaled = normal * 123.0;
		PolygonNormal n = {};
		n.normal = normalScaled;
		n.sortInfo = 0;
		n.nPower = 0;
		normals.push_back(n);
	}

	for(const Group& group: obj.groups){
		ColorId colorId = ColorId::BODY;

		if(importSettings.materialsMapping.contains(group.material)){
			colorId = importSettings.materialsMapping[group.material];
		}

		for(const Face& face1: group.faces){
			for(const Face& face: triangulate(face1)){
				Polygon polygon {
					.num = face.indices.size(),
					.colorId = (uint32_t)colorId,
				};

				for(const FaceIndex& index: face.indices){
					polygon.indices.push_back({
						.vertInd = index.vertexIndex,
						.normInd = index.normalIndex,
					});
				}
				std::reverse(polygon.indices.begin(), polygon.indices.end());

				const Vector3F32& v1 = vertices[polygon.indices[0].vertInd].tf;
				const Vector3F32& v2 = vertices[polygon.indices[1].vertInd].tf;
				const Vector3F32& v3 = vertices[polygon.indices[2].vertInd].tf;

				Vector3F32 norm = (v2 - v1).cross(v2 - v3).normalized();
				Normal flatNormal = {
					.normal = (Vector3I8)(norm * 123),
					.nPower = 8
				};

				Vector3F64 middle = {};
				for(const PolygonIndex& index: polygon.indices) {
					middle = middle + vertices[index.vertInd].tf;
				}
				middle = middle / 4;

				polygon.colorId = (uint32_t)colorId;
				polygon.flatNormal = flatNormal;
				polygon.middle = middle;

				polygons.push_back(polygon);
			}

		}

	}

	std::vector<double> lengths;
	lengths.reserve(vertices.size());
	std::transform(vertices.begin(), vertices.end(), std::back_inserter(lengths), [](const Vertex& v){
		return v.tf.length();
	});

	double maxLength = *std::max_element(lengths.begin(), lengths.end());
	int numVertTotal = std::accumulate(polygons.begin(), polygons.end(), 0, [](int a, const Polygon& b){
		return a + b.num;
	});

	c3d.version = 8,
	c3d.numVert = (int32_t)vertices.size();
	c3d.numNorm = (int32_t)normals.size();
	c3d.numPoly = (int32_t)polygons.size();
	c3d.numVertTotal = numVertTotal;
	c3d.maxBound = bounds.max * scaleFactor;
	c3d.minBound = bounds.min * scaleFactor;
	c3d.angle = {83, 83, 83};
	c3d.rmax = maxLength;
	c3d.offsets = origin;
	c3d.vectices = vertices;
	c3d.normals = normals;
	c3d.polygons = polygons;
	c3d.polyInds[0] = std::vector<int32_t>(polygons.size());
	c3d.polyInds[1] = std::vector<int32_t>(polygons.size());
	c3d.polyInds[2] = std::vector<int32_t>(polygons.size());
	c3d.J = Matrix3x3F64::identity();
	c3d.rcm = {};
	c3d.volume = 0;
}

Bounds objectBounds(const Object& obj){
	if(obj.vertices.size() == 0) {
		return {};
	}

	Vector3F64 minPos = obj.vertices[0];
	Vector3F64 maxPos = obj.vertices[1];

	for(const Vector3F64& v: obj.vertices){
		minPos.x = std::min(v.x, minPos.x);
		minPos.y = std::min(v.y, minPos.y);
		minPos.z = std::min(v.z, minPos.z);

		maxPos.x = std::max(v.x, maxPos.x);
		maxPos.y = std::max(v.y, maxPos.y);
		maxPos.z = std::max(v.z, maxPos.z);
	}
	return {minPos, maxPos};
}


bool ObjImporter::import(const ObjectCollection& objCollection,
			const ObjImportSettings& importSettings,
			M3D& m3d)
{
	m3d = {};


	QList<const Object*> objs;
	for(const Object& obj: objCollection.objects){
		ObjectType objType = importSettings.objectMapping[obj.name];
		if(objType == ObjectType::Body){
			objs.append(&obj);
		}
	}

	Object bodyGeometry {};
	Object::merge(bodyGeometry, objs);


	Vector3F64 minPos = bodyGeometry.vertices[0];
	Vector3F64 maxPos = bodyGeometry.vertices[1];

	for(const Vector3F64& v: bodyGeometry.vertices){
		minPos.x = std::min(v.x, minPos.x);
		minPos.y = std::min(v.y, minPos.y);
		minPos.z = std::min(v.z, minPos.z);

		maxPos.x = std::max(v.x, maxPos.x);
		maxPos.y = std::max(v.y, maxPos.y);
		maxPos.z = std::max(v.z, maxPos.z);
	}

	Vector3F64 delta = maxPos - minPos;
	double maxDim = std::max(delta.x, std::max(delta.y, delta.z));

	double scaleFactor = 255.0 / maxDim;
	if(importSettings.customScale.hasValue()){
		scaleFactor = importSettings.customScale;
	}

	Bounds bounds{
		.min = minPos,
		.max = maxPos,
	};
	Vector3F64 bodyOrigin = bounds.origin();

	bool isMechous = importSettings.objectMapping.values().contains(ObjectType::Wheel);


	objectToC3D(bodyGeometry, m3d.body, importSettings, scaleFactor, bounds);
	calcPhysics(m3d.body);
	if(importSettings.customVolume.hasValue()){
		m3d.body.volume = importSettings.customVolume;
	}

	if(importSettings.customRcm.hasValue()){
		m3d.body.rcm = importSettings.customRcm;
	}

	if(importSettings.customJ.hasValue()){
		m3d.body.J = importSettings.customJ;
	}

	bool boundFound = false;

	for(const Object& obj: objCollection.objects){
		if(importSettings.objectMapping[obj.name] != ObjectType::BodyBound) continue;
		boundFound = true;
		Bounds b = objectBounds(obj);
		objectToC3D(obj, m3d.bound, importSettings, scaleFactor, b);

		calcPhysics(m3d.bound);
	}

	if(!boundFound){
		Object bodyBoundsGeometry;
		makeBoundingBox(bodyBoundsGeometry, bounds);
		skinBodyBounds(bodyGeometry, bodyBoundsGeometry);
		objectToC3D(bodyBoundsGeometry, m3d.bound, importSettings, scaleFactor, bounds);
		alterBoundingBox(m3d.bound, m3d.body, isMechous);
		calcPhysics(m3d.bound);
	}



	// TODO:
	m3d.bodyColorOffset = 144;
	m3d.bodyColorShift = 3;

	m3d.rMax = m3d.body.rmax;
	m3d.bounds = m3d.body.maxBound;

	for(const Object& obj: objCollection.objects){
		ObjectType type = importSettings.objectMapping[obj.name] ;
		if(type != ObjectType::WheelStatic &&
		   type != ObjectType::Wheel) continue;

		Bounds wheelBounds = objectBounds(obj);

		int32_t steer = type == ObjectType::Wheel ? 1 : 0;

		Wheel wheel{
			.steer = steer,
			.f = (wheelBounds.origin() - bodyOrigin) * scaleFactor,
			.width = (int32_t)((wheelBounds.max.x - wheelBounds.min.x) * scaleFactor),
			.radius = (int32_t)((wheelBounds.max.y - wheelBounds.min.y) * scaleFactor),
			.boundIndex = 0,
			.model = {}
		};

		if(steer){
			wheel.model.push_back({});
			objectToC3D(obj, wheel.model[0], importSettings, scaleFactor, wheelBounds);
			wheel.model[0].offsets = wheel.f;
		}

		m3d.wheels.push_back(wheel);
		m3d.nWheels += 1;
	}
	std::sort(m3d.wheels.begin(), m3d.wheels.end(), [](const Wheel& w1, const Wheel& w2){ return w1.steer > w2.steer; });

	for(const Object& obj: objCollection.objects){
		if(importSettings.objectMapping[obj.name] != ObjectType::DebreeBound) continue;

		Bounds b = objectBounds(obj);
		C3D debreeBounds;
		objectToC3D(obj, debreeBounds, importSettings, scaleFactor, b);
		calcPhysics(debreeBounds);
		m3d.boundDebris.push_back(debreeBounds);
	}

	for(const Object& obj: objCollection.objects){
		if(importSettings.objectMapping[obj.name] != ObjectType::Debree) continue;

		Bounds b = objectBounds(obj);
		C3D debree;
		objectToC3D(obj, debree, importSettings, scaleFactor, b);

		if(m3d.boundDebris.size() == 0){
			Object boundsGeometry;
			makeBoundingBox(boundsGeometry, b);

			C3D debreeBounds;
			objectToC3D(boundsGeometry, debreeBounds, importSettings, scaleFactor, b);
			alterBoundingBox(debreeBounds, m3d.body, isMechous);
			calcPhysics(debreeBounds);
			m3d.boundDebris.push_back(debreeBounds);
		}

		m3d.debris.push_back(debree);

	}
	m3d.nDebris = m3d.debris.size();


	m3d.rSlots.resize(3);
	for(const Object& obj: objCollection.objects){
		ObjectType type = importSettings.objectMapping[obj.name];

		if(type == ObjectType::Slot1 ||
		   type == ObjectType::Slot2 ||
		   type == ObjectType::Slot3){

			Bounds b = objectBounds(obj);
			Slot slot {
				.rSlots = (b.origin() - bodyOrigin) * scaleFactor,
				.locationAngleOfSlots = 0, //TODO:
			};

			int shift = 0;
			if(type == ObjectType::Slot1){
				shift = 0;
			}

			if(type == ObjectType::Slot2){
				shift = 1;
			}

			if(type == ObjectType::Slot3){
				shift = 2;
			}

			m3d.slotsExistence |= (1 << shift);
			m3d.rSlots[shift] = slot;
		}

	}

	return true;
}

void skinBodyBounds(const Object& body, Object& bounds){
	int nClosest = 8;

	QList<double> distances;
	QList<int32_t> indices;
	distances.reserve(body.vertices.size());
	indices.reserve(body.vertices.size());

	for(int i = 0; i < bounds.vertices.size(); i++){
		Vector3F64& boundVertex = bounds.vertices[i];
		Vector3F64 initialVertex = boundVertex;

		distances.clear();
		indices.clear();
		for(const Vector3F64& bodyVertex: body.vertices){
			double distance = (boundVertex - bodyVertex).lengthSquared();
			distances.append(distance);
			indices.append(indices.size());
		}
		std::sort(indices.begin(), indices.end(), [&distances](auto x, auto y){
			return distances[x] < distances[y];
		});

		Vector3F64 mid = initialVertex;
		int count = 0;
		for(int j = 0; j < nClosest && j < body.vertices.size(); j++){
			mid = mid + body.vertices[indices[j]];
			count += 1;
		}
		mid = mid / count;
		boundVertex = mid;
		switch (i) {
		case 0:
		case 1:
		case 2:
		case 6:
		case 7:
		case 8:
			boundVertex.x = initialVertex.x;
			boundVertex.z = initialVertex.z;
			break;
		case 3:
		case 4:
		case 5:
			boundVertex.z = initialVertex.z;
		}

	}
}

void alterBoundingBox(C3D& bounds, C3D& body, bool isMechous){

	for(Polygon& polygon: bounds.polygons){
		ColorId colorId = ColorId::DEFENSE;
		const Vector3F32& v1 = bounds.vectices[polygon.indices[0].vertInd].tf;
		const Vector3F32& v2 = bounds.vectices[polygon.indices[1].vertInd].tf;
		const Vector3F32& v3 = bounds.vectices[polygon.indices[2].vertInd].tf;

		Vector3F32 norm = (v2 - v1).cross(v2 - v3).normalized();
		Normal flatNormal = {
			.normal = (Vector3I8)(norm * 123),
			.nPower = 8
		};

		Vector3F64 middle = {};
		for(const PolygonIndex& index: polygon.indices) {
			middle = middle + bounds.vectices[index.vertInd].tf;
		}
		middle = middle / 4;


		if(isMechous) {
			bool isLowerPlane = flatNormal.normal.x == 0 && flatNormal.normal.y == 0 && flatNormal.normal.z < 0;
			colorId = isLowerPlane ? ColorId::ZERO_RESERVED : ColorId::BODY;

			if(isLowerPlane) {
				Vector3F32 maxAbsVertex = bounds.vectices[polygon.indices[0].vertInd].tf;
				for(int i = 1; i < polygon.indices.size(); i++){
					Vector3F32 v = bounds.vectices[polygon.indices[i].vertInd].tf;

					if(std::abs(maxAbsVertex.x) < std::abs(v.x)){
						maxAbsVertex.x = v.x;
					}

					if(std::abs(maxAbsVertex.y) < std::abs(v.y)){
						maxAbsVertex.y = v.y;
					}

					if(std::abs(maxAbsVertex.z) < std::abs(v.z)){
						maxAbsVertex.z = v.z;
					}
				}

				middle.x = body.maxBound.x;
				middle.y = body.maxBound.y;
				if(maxAbsVertex.x < 0) {
					middle.x *= -1;
				}

				if(maxAbsVertex.y < 0) {
					middle.y *= -1;
				}
				middle.z = body.minBound.z;
			}
			polygon.colorId = (uint32_t)colorId;
			polygon.flatNormal = flatNormal;
			polygon.middle = middle;

		}


	}
}

void makeBoundingBox(Object& result, const Bounds& bounds){
	Vector3F64 mid = bounds.origin();
	const Vector3F64& min = bounds.min;
	const Vector3F64& max = bounds.max;
	result.vertices = {
		// Lower plane
		{min.x, min.y, min.z}, // 0
		{min.x, mid.y, min.z}, // 1
		{min.x, max.y, min.z}, // 2

		{mid.x, min.y, min.z}, // 3
		{mid.x, mid.y, min.z}, // 4
		{mid.x, max.y, min.z}, // 5

		{max.x, min.y, min.z}, // 6
		{max.x, mid.y, min.z}, // 7
		{max.x, max.y, min.z}, // 8

		// Mid plane
		{min.x, min.y, mid.z}, // 9
		{min.x, mid.y, mid.z}, // 10
		{min.x, max.y, mid.z}, // 11

		{mid.x, min.y, mid.z}, // 12
		{mid.x, mid.y, mid.z}, // 13
		{mid.x, max.y, mid.z}, // 14

		{max.x, min.y, mid.z}, // 15
		{max.x, mid.y, mid.z}, // 16
		{max.x, max.y, mid.z}, // 17

		// Upper plane
		{min.x, min.y, max.z}, // 18
		{min.x, mid.y, max.z}, // 19
		{min.x, max.y, max.z}, // 20

		{mid.x, min.y, max.z}, // 21
		{mid.x, mid.y, max.z}, // 22
		{mid.x, max.y, max.z}, // 23

		{max.x, min.y, max.z}, // 24
		{max.x, mid.y, max.z}, // 25
		{max.x, max.y, max.z}, // 26
	};
	result.normals = {
		{-1, 0, 0}, // 0
		{1, 0, 0}, // 1
		{0, -1, 0}, // 2
		{0, 1, 0}, // 3
		{0, 0, -1}, // 4
		{0, 0, 1} // 5
	};

	result.groups.append(Group{});

	result.groups.last().faces = {
		{{{4, 0, 4}, {3, 0, 4}, {0, 0, 4}, {1, 0, 4}}},
		{{{5, 0, 4}, {4, 0, 4}, {1, 0, 4}, {2, 0, 4}}},
		{{{8, 0, 4}, {7, 0, 4}, {4, 0, 4}, {5, 0, 4}}},
		{{{7, 0, 4}, {6, 0, 4}, {3, 0, 4}, {4, 0, 4}}},

		{{{19, 0, 5}, {18, 0, 5}, {21, 0, 5}, {22, 0, 5}}},
		{{{22, 0, 5}, {21, 0, 5}, {24, 0, 5}, {25, 0, 5}}},
		{{{23, 0, 5}, {22, 0, 5}, {25, 0, 5}, {26, 0, 5}}},
		{{{20, 0, 5}, {19, 0, 5}, {22, 0, 5}, {23, 0, 5}}},

		{{{26, 0, 1}, {25, 0, 1}, {16, 0, 1}, {17, 0, 1}}},
		{{{25, 0, 1}, {24, 0, 1}, {15, 0, 1}, {16, 0, 1}}},
		{{{17, 0, 1}, {16, 0, 1}, {7, 0, 1}, {8, 0, 1}}},
		{{{16, 0, 1}, {15, 0, 1}, {6, 0, 1}, {7, 0, 1}}},

		{{{20, 0, 3}, {23, 0, 3}, {14, 0, 3}, {11, 0, 3}}},
		{{{23, 0, 3}, {26, 0, 3}, {17, 0, 3}, {14, 0, 3}}},
		{{{14, 0, 3}, {17, 0, 3}, {8, 0, 3}, {5, 0, 3}}},
		{{{11, 0, 3}, {14, 0, 3}, {5, 0, 3}, {2, 0, 3}}},

		{{{18, 0, 0}, {19, 0, 0}, {10, 0, 0}, {9, 0, 0}}},
		{{{19, 0, 0}, {20, 0, 0}, {11, 0, 0}, {10, 0, 0}}},
		{{{10, 0, 0}, {11, 0, 0}, {2, 0, 0}, {1, 0, 0}}},
		{{{9, 0, 0}, {10, 0, 0}, {1, 0, 0}, {0, 0, 0}}},

		{{{24, 0, 2}, {21, 0, 2}, {12, 0, 2}, {15, 0, 2}}},
		{{{21, 0, 2}, {18, 0, 2}, {9, 0, 2}, {12, 0, 2}}},
		{{{15, 0, 2}, {12, 0, 2}, {3, 0, 2}, {6, 0, 2}}},
		{{{12, 0, 2}, {9, 0, 2}, {0, 0, 2}, {3, 0, 2}}},
	};
//	QList<Face> faces {
//		{{{3, 0, 4}, {0, 0, 4}, {1, 0, 4}}},
//		{{{4, 0, 4}, {3, 0, 4}, {1, 0, 4}}},
//		{{{6, 0, 4}, {3, 0, 4}, {4, 0, 4}}},
//		{{{7, 0, 4}, {6, 0, 4}, {4, 0, 4}}},
//		{{{4, 0, 4}, {1, 0, 4}, {2, 0, 4}}},
//		{{{5, 0, 4}, {4, 0, 4}, {2, 0, 4}}},
//		{{{7, 0, 4}, {4, 0, 4}, {5, 0, 4}}},
//		{{{8, 0, 4}, {7, 0, 4}, {5, 0, 4}}},

//		{{{19, 0, 5}, {18, 0, 5}, {21, 0, 5}}},
//		{{{19, 0, 5}, {21, 0, 5}, {22, 0, 5}}},
//		{{{22, 0, 5}, {21, 0, 5}, {24, 0, 5}}},
//		{{{22, 0, 5}, {24, 0, 5}, {25, 0, 5}}},
//		{{{20, 0, 5}, {19, 0, 5}, {22, 0, 5}}},
//		{{{20, 0, 5}, {22, 0, 5}, {23, 0, 5}}},
//		{{{23, 0, 5}, {22, 0, 5}, {25, 0, 5}}},
//		{{{23, 0, 5}, {25, 0, 5}, {26, 0, 5}}},

//		{{{25, 0, 1}, {24, 0, 1}, {15, 0, 1}}},
//		{{{25, 0, 1}, {15, 0, 1}, {16, 0, 1}}},
//		{{{16, 0, 1}, {15, 0, 1}, {6, 0, 1}}},
//		{{{16, 0, 1}, {6, 0, 1}, {7, 0, 1}}},
//		{{{26, 0, 1}, {25, 0, 1}, {16, 0, 1}}},
//		{{{26, 0, 1}, {16, 0, 1}, {17, 0, 1}}},
//		{{{17, 0, 1}, {16, 0, 1}, {7, 0, 1}}},
//		{{{17, 0, 1}, {7, 0, 1}, {8, 0, 1}}},

//		{{{23, 0, 3}, {26, 0, 3}, {17, 0, 3}}},
//		{{{23, 0, 3}, {17, 0, 3}, {14, 0, 3}}},
//		{{{14, 0, 3}, {17, 0, 3}, {8, 0, 3}}},
//		{{{14, 0, 3}, {8, 0, 3}, {5, 0, 3}}},
//		{{{20, 0, 3}, {23, 0, 3}, {14, 0, 3}}},
//		{{{20, 0, 3}, {14, 0, 3}, {11, 0, 3}}},
//		{{{11, 0, 3}, {14, 0, 3}, {5, 0, 3}}},
//		{{{11, 0, 3}, {5, 0, 3}, {2, 0, 3}}},

//		{{{19, 0, 0}, {20, 0, 0}, {11, 0, 0}}},
//		{{{19, 0, 0}, {11, 0, 0}, {10, 0, 0}}},
//		{{{10, 0, 0}, {11, 0, 0}, {2, 0, 0}}},
//		{{{10, 0, 0}, {2, 0, 0}, {1, 0, 0}}},
//		{{{18, 0, 0}, {19, 0, 0}, {10, 0, 0}}},
//		{{{18, 0, 0}, {10, 0, 0}, {9, 0, 0}}},
//		{{{9, 0, 0}, {10, 0, 0}, {1, 0, 0}}},
//		{{{9, 0, 0}, {1, 0, 0}, {0, 0, 0}}},

//		{{{21, 0, 2}, {18, 0, 2}, {9, 0, 2}}},
//		{{{21, 0, 2}, {9, 0, 2}, {12, 0, 2}}},
//		{{{12, 0, 2}, {9, 0, 2}, {0, 0, 2}}},
//		{{{12, 0, 2}, {0, 0, 2}, {3, 0, 2}}},
//		{{{24, 0, 2}, {21, 0, 2}, {12, 0, 2}}},
//		{{{24, 0, 2}, {12, 0, 2}, {15, 0, 2}}},
//		{{{15, 0, 2}, {12, 0, 2}, {3, 0, 2}}},
//		{{{15, 0, 2}, {3, 0, 2}, {6, 0, 2}}},
//	};
}



void ObjImporter::makeDefaultImportSettings(const ObjectCollection& objCollection, ObjImportSettings& importSettings)
{
	for(const Object& obj: objCollection.objects){
		importSettings.objectMapping[obj.name] = ObjectType::Body;
		for(const Group& group: obj.groups){
			importSettings.materialsMapping[group.material] = ColorId::BODY;
		}
	}
}

