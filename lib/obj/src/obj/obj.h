#ifndef OBJ_H
#define OBJ_H

#include <vangers/core//vector/vector.h>
#include <vangers/core//optional.h>
#include <QList>

namespace obj {
	struct FaceIndex {
		vangers::core::optional::Optional<int32_t> vertexIndex;
		vangers::core::optional::Optional<int32_t> textureCoordIndex;
		vangers::core::optional::Optional<int32_t> normalIndex;
	};

	using Vector3F64 = vangers::core::vector::Vector3<double>;
	using Vector3F32 = vangers::core::vector::Vector3<float>;
	using Size3F64 = vangers::core::vector::Size3<double>;

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
		void triangulate();
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

		Size3F64 bounds() const;

		void move(const Vector3F64& origin);
	};
}

#endif // OBJ_H
