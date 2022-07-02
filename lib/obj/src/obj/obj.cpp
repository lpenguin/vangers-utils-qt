#include "obj.h"

#include <QMap>

using namespace obj;


void Object::move(const Vector3F64& origin)
{
	for(Vector3F64& v: vertices){
		v = v + origin;
	}
}

Size3F64 Object::bounds() const {
	if(vertices.size() == 0) {
		return {};
	}

	Vector3F64 minPos = vertices[0];
	Vector3F64 maxPos = vertices[0];

	for(const Vector3F64& v: vertices){
		minPos.x = std::min(v.x, minPos.x);
		minPos.y = std::min(v.y, minPos.y);
		minPos.z = std::min(v.z, minPos.z);

		maxPos.x = std::max(v.x, maxPos.x);
		maxPos.y = std::max(v.y, maxPos.y);
		maxPos.z = std::max(v.z, maxPos.z);
	}
	return {.min = minPos, .max = maxPos};
}

void Object::merge(Object& result, const QList<const Object*>& objects)
{
	for(const Object* obj: objects){
		int32_t verticesCount = result.vertices.size();
		int32_t normalsCount = result.normals.size();

		result.vertices.append(obj->vertices);
		result.normals.append(obj->normals);
		for(const Group& group: obj->groups){
			result.groups.append(Group{
				 .name = group.name,
				 .material = group.material,
			 });

			Group& newGroup = result.groups.last();
			for(const Face& f: group.faces){
				newGroup.faces.append(Face{});
				Face& newFace = newGroup.faces.last();
				for(const FaceIndex& index: f.indices){
					newFace.indices.append(FaceIndex{
						.vertexIndex = verticesCount + index.vertexIndex,
						.normalIndex = normalsCount + index.normalIndex,
					});
				}
			}

		}
	}
}

void Object::makeCube(Object& result, const Vector3F64& size, const Vector3F64& origin)
{
	double s = 0.5f;
	result.vertices = {
		(origin + Vector3F64{-s, s, -s} * size),
		(origin + Vector3F64{-s, -s, -s} * size),
		(origin + Vector3F64{s, -s, -s} * size),
		(origin + Vector3F64{s, s, -s} * size),
		(origin + Vector3F64{-s, s, s} * size),
		(origin + Vector3F64{-s, -s, s} * size),
		(origin + Vector3F64{s, -s, s} * size),
		(origin + Vector3F64{s, s, s} * size),
	};

	result.normals = {
		{-1, 0, 0},
		{1, 0, 0},
		{0, -1, 0},
		{0, 1, 0},
		{0, 0, -1},
		{0, 0, 1}
	};


	result.groups = {Group{
		.faces	= {
			{{{1, 0, 0}, {5, 0, 0}, {4, 0, 0}, {0, 0, 0}}},
			{{{7, 0, 1}, {6, 0, 1}, {2, 0, 1}, {3, 0, 1}}},
			{{{6, 0, 2}, {5, 0, 2}, {1, 0, 2}, {2, 0, 2}}},
			{{{0, 0, 3}, {4, 0, 3}, {7, 0, 3}, {3, 0, 3}}},
			{{{0, 0, 4}, {3, 0, 4}, {2, 0, 4}, {1, 0, 4}}},
			{{{4, 0, 5}, {5, 0, 5}, {6, 0, 5}, {7, 0, 5}}},
		}
					 }};
}

void Object::triangulate()
{
	for(Group& g: groups){
		QList<Face> newFaces;
		for(const Face& face: g.faces){
			if(face.indices.size() == 3) {
				newFaces.append(face);
			} else {
				FaceIndex i0 = face.indices[0];
				for(int i = 2; i< face.indices.size(); i++){
					const FaceIndex& iCurr = face.indices[i];
					const FaceIndex& iPrev = face.indices[i - 1];
					newFaces.append({{i0, iPrev, iCurr}});
				}
			}
		}
		g.faces = newFaces;
	}
}

Size3F64 ObjectCollection::bounds() const
{
	if(objects.size() == 0) {
		return {};
	}

	Size3F64 res = objects[0].bounds();
	for(int i = 1; i < objects.size(); i++){
		Size3F64 bounds = objects[i].bounds();
		res.min.x = std::min(res.min.x, bounds.min.x);
		res.min.y = std::min(res.min.y, bounds.min.y);
		res.min.z = std::min(res.min.z, bounds.min.z);

		res.max.x = std::max(res.max.x, bounds.max.x);
		res.max.y = std::max(res.max.y, bounds.max.y);
		res.max.z = std::max(res.max.z, bounds.max.z);
	}

	return res;
}

void ObjectCollection::move(const Vector3F64& origin)
{
	for(Object& obj: objects){
		obj.move(origin);
	}
}
