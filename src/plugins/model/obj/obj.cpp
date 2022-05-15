#include "obj.h"

#include <QMap>

using namespace vangers::model::obj;


void Object::move(const Vector3F64& origin)
{
	for(Vector3F64& v: vertices){
		v = v + origin;
	}
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
