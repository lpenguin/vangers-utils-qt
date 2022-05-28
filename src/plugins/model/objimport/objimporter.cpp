#include "objimporter.h"
#include <QSet>
#include <volint/volint.h>
#include <mtool/ModelClass.h>
#include <QDebug>

using namespace vangers::model::objimport;

void makeBoundingBox(Object& result,const Size3F64& bounds);
void skinBodyBounds(const Object& body, Object& bounds);
void alterBoundingBox(C3D& bounds, C3D& body, bool isMechous);
void calcPhysics(C3D& c3d);
void calcPhysicsMtool(C3D& c3d);


void calcPhysicsMtool(C3D& c3d){
	mtool::Vertex* vertices = new mtool::Vertex[c3d.vectices.size()];
	mtool::Face* faces = new mtool::Face[c3d.polygons.size()];

	for(size_t i = 0; i < c3d.vectices.size(); i++){
		const Vector3F32& v = c3d.vectices[i].tf;
		vertices[i][0] = v.x;
		vertices[i][1] = v.y;
		vertices[i][2] = v.z;
	}

	for(size_t i = 0; i < c3d.polygons.size(); i++){
		const Polygon& p = c3d.polygons[i];
		if(p.num != 3) {
			faces[i] = mtool::Face{};
		} else {
			faces[i] = mtool::Face {
				.Vertices = {
					(uint32_t) p.indices[0].vertInd,
					(uint32_t) p.indices[1].vertInd,
					(uint32_t) p.indices[2].vertInd
				},
	//			.Normals = {
	//				(uint32_t) p.indices[0].normInd,
	//				(uint32_t) p.indices[1].normInd,
	//				(uint32_t) p.indices[2].normInd
	//			}
			};
		}


	}

	mtool::Model model;
	model.VerticesCount = c3d.vectices.size();
	model.Vertices = vertices;
	model.FacesCount = c3d.polygons.size();
	model.Faces = faces;

	mtool::PhysicalParamsStruct pps {};

	model.MakeFacesNormals();
//	model.MakeVerticesNormals();
	model.CalculateDims();
	model.CalculatePhysicalParams(pps, false);
	c3d.volume = pps.Volume;
	c3d.rcm.x = pps.MassCenter[0];
	c3d.rcm.y = pps.MassCenter[1];
	c3d.rcm.z = pps.MassCenter[2];

//	Vector3F64 geomCenter = Vector3F64::fromVector((c3d.minBound + c3d.maxBound) / 2.0f);
//	c3d.rcm = c3d.rcm + geomCenter;

	for(int i = 0; i < 9; i++){
		c3d.J.values[i] = pps.IntertiaTensor[i];
	}
}

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
		Size3F64 bounds){

	std::vector<Vertex> vertices;
	std::vector<PolygonNormal> normals;
	std::vector<Polygon> polygons;

//	Vector3F64 origin = (bounds.max + bounds.min) / 2.0;

	for(const Vector3F64& vertex: obj.vertices){
		Vector3F64 objVertexScaled = (vertex) * scaleFactor;

		Vertex v {
			.tf = Vector3F32::fromVector(objVertexScaled),
			.pos = Vector3I8::round(objVertexScaled),
			.sortInfo = 0,
		};
		vertices.push_back(v);
	}

	for(const Vector3F64& normal: obj.normals) {
		Vector3F64 normalScaled = normal * 123.0;
		PolygonNormal n = {};
		n.normal = Vector3I8::round(normalScaled);
		n.sortInfo = 0;
		n.nPower = 0;
		normals.push_back(n);
	}

	for(const Group& group: obj.groups){
		ColorId colorId = ColorId::BODY;

		if(importSettings.materialsMapping.contains(group.material)){
			colorId = importSettings.materialsMapping[group.material];
		}

		for(const Face& face: group.faces){
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

			Vector3F32 norm = (v1 - v2).cross(v3 - v1).normalized();
			Vector3F32 normScaled = norm * 123.0f;

			Normal flatNormal = {
				.normal = Vector3I8::round(normScaled),
				.nPower = 0
			};

			Vector3F32 middle = {};
			for(const PolygonIndex& index: polygon.indices) {
				middle = middle + vertices[index.vertInd].tf;
			}
			middle = middle / polygon.indices.size();

			polygon.colorId = (uint32_t)colorId;
			polygon.flatNormal = flatNormal;
			polygon.middle = Vector3I8::round(middle);

			polygons.push_back(polygon);
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
	c3d.maxBound = Vector3I32::round(bounds.max * scaleFactor);
	c3d.minBound = Vector3I32::round(bounds.min * scaleFactor);
	c3d.angle = {83, 83, 83};
	c3d.rmax = qRound(maxLength);
	c3d.offsets = {0, 0, 0};
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
	if(importSettings.triangulate){
		bodyGeometry.triangulate();
	}

	Size3F64 bounds = bodyGeometry.bounds();
//	Vector3F64 bodyOrigin = bounds.mid();
//	Vector3F64 bodySize = bounds.size();

//	double maxDim = std::max(bodySize.x, std::max(bodySize.y, bodySize.z));
	double maxDimMax = std::max(std::abs(bounds.max.x), std::max(std::abs(bounds.max.y), std::abs(bounds.max.z)));
	double maxDimMin = std::max(std::abs(bounds.min.x), std::max(std::abs(bounds.min.y), std::abs(bounds.min.z)));
	double maxDim = std::max(maxDimMax, maxDimMin) * 2;

	double scaleFactor = 255.0 / maxDim;
	if(importSettings.customScale.hasValue()){
		scaleFactor = importSettings.customScale;
	}

	bool isMechous = importSettings.objectMapping.values().contains(ObjectType::Wheel);

	objectToC3D(bodyGeometry, m3d.body, importSettings, scaleFactor, bounds);
	calcPhysicsMtool(m3d.body);
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
		objectToC3D(obj, m3d.bound, importSettings, scaleFactor, obj.bounds());
	}

	if(!boundFound){
		Object bodyBoundsGeometry;
		makeBoundingBox(bodyBoundsGeometry, bounds);
		skinBodyBounds(bodyGeometry, bodyBoundsGeometry);
		objectToC3D(bodyBoundsGeometry, m3d.bound, importSettings, scaleFactor, bounds);
	}

	alterBoundingBox(m3d.bound, m3d.body, isMechous);
	m3d.bound.rcm = m3d.body.rcm;
	m3d.bound.J = m3d.body.J;
	m3d.bound.volume = 0;

	// TODO:
//	calcPhysicsMtool(m3d.bound);

	m3d.bodyColorOffset = importSettings.bodyColorOffset;
	m3d.bodyColorShift = importSettings.bodyColorShift;

	m3d.rMax = m3d.body.rmax;
	m3d.bounds = m3d.body.maxBound;

	for(const Object& obj: objCollection.objects){
		ObjectType type = importSettings.objectMapping[obj.name];

		if(type != ObjectType::WheelStatic &&
		   type != ObjectType::Wheel) continue;

		Size3F64 wheelBounds = obj.bounds();
		Vector3F64 wheelOrigin = wheelBounds.mid();
		Vector3I32 wheelSize = Vector3I32::round(wheelBounds.size() * scaleFactor);

		int32_t steer = type == ObjectType::Wheel ? 1 : 0;

		Wheel wheel{
			.steer = steer,
			.f = wheelOrigin * scaleFactor,
			.width = wheelSize.x,
			.radius = wheelSize.y,
			.boundIndex = 0,
			.model = {}
		};

		if(steer){
			wheel.model.push_back({});
			Object centeredObj = obj;
			centeredObj.move(wheelOrigin * -1);
			objectToC3D(centeredObj, wheel.model[0], importSettings, scaleFactor, centeredObj.bounds());
			wheel.model[0].offsets = Vector3I32::round(wheel.f);
		}

		m3d.wheels.push_back(wheel);
		m3d.nWheels += 1;
	}
	std::sort(m3d.wheels.begin(), m3d.wheels.end(), [](const Wheel& w1, const Wheel& w2){ return w1.steer > w2.steer; });

	for(const Object& obj: objCollection.objects){
		if(importSettings.objectMapping[obj.name] != ObjectType::DebreeBound) continue;

		Size3F64 b = obj.bounds();
		C3D debreeBounds;
		objectToC3D(obj, debreeBounds, importSettings, scaleFactor, b);
		calcPhysics(debreeBounds);
		m3d.boundDebris.push_back(debreeBounds);
	}

	for(const Object& obj: objCollection.objects){
		if(importSettings.objectMapping[obj.name] != ObjectType::Debree) continue;

		Size3F64 b = obj.bounds();
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

			Size3F64 b = obj.bounds();
			Slot slot {
				.rSlots = Vector3I32::round(b.mid() * scaleFactor),
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
		const Vector3I8& flatNormal = polygon.flatNormal.normal;

		if(isMechous) {
			bool isLowerPlane = flatNormal.x == 0 && flatNormal.y == 0 && flatNormal.z < 0;
//			colorId = isLowerPlane ? ColorId::ZERO_RESERVED : ColorId::BODY;
			if(isLowerPlane) {
				polygon.colorId = (uint32_t)ColorId::ZERO_RESERVED;
			}

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

				polygon.middle.x = body.maxBound.x;
				polygon.middle.y = body.maxBound.y;
				if(maxAbsVertex.x < 0) {
					polygon.middle.x *= -1;
				}

				if(maxAbsVertex.y < 0) {
					polygon.middle.y *= -1;
				}
				polygon.middle.z = body.minBound.z;
			}
		}
	}
}

void makeBoundingBox(Object& result, const Size3F64& bounds){
	Vector3F64 mid = bounds.mid();
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
}



void ObjImporter::makeDefaultImportSettings(const ObjectCollection& objCollection, ObjImportSettings& importSettings)
{
	for(const Object& obj: objCollection.objects){
		importSettings.objectMapping[obj.name] = ObjectType::Body;
		for(const Group& group: obj.groups){
			importSettings.materialsMapping[group.material] = ColorId::BODY;
		}
	}
	importSettings.triangulate = true;
	importSettings.bodyColorOffset = 144;
	importSettings.bodyColorShift = 3;
}

