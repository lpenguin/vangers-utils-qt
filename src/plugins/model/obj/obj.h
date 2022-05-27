#ifndef OBJ_H
#define OBJ_H

#include <core/vector/vector.h>
#include <core/optional.h>
#include <QList>

namespace vangers::model::obj {
	using namespace vangers::core::vector;
	using namespace vangers::core::optional;

	struct FaceIndex {	
		Optional<int32_t> vertexIndex;
		Optional<int32_t> textureCoordIndex;
		Optional<int32_t> normalIndex;
	};

	struct Face {
		QList<FaceIndex> indices;
	};

	struct Group {
		QString name;
		QString material;
		QList<Face> faces;
	};

	struct Object {
		QString name;
		QList<Vector3F64> vertices;
		QList<Vector3F64> normals;
		QList<Group> groups;

		void move(const Vector3F64& origin);
		Size3F64 bounds() const;
		static void merge(Object& result, const QList<const Object*>& objects);
		static void makeCube(Object& result, const Vector3F64& size, const Vector3F64& origin);
	};

	struct Material {
		QString name;
		Vector3F32 ambientColor;
		Vector3F32 diffuseColor;
		Vector3F32 specularColor;
	};

	struct ObjectCollection {
		QStringList useMaterialLibraries;
		QList<Material> materials;
		QList<Object> objects;

////		ObjectCollection subset(const QStringList& names) const;
//		static ObjectCollection merge(const QList<ObjectCollection>& collections);
	};
}

#endif // OBJ_H
