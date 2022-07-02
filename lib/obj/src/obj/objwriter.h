#ifndef OBJWRITER_H
#define OBJWRITER_H

#include <QIODevice>
#include <QTextStream>
#include "obj.h"

namespace obj {
	class ObjStreamWriter
	{
	public:
		ObjStreamWriter(QIODevice* ioDevice);
		void writeStartObject(const QString& name);
		void writeStartGroup(const QString& name);
		void writeUseMaterial(const QString& name);
		void writeUseMaterialLib(const QString& fileName);
		void writeVertex(qreal x, qreal y, qreal z);
		void writeNormal(qreal x, qreal y, qreal z);
		void writeFace(const QList<FaceIndex>& indices, int vertexOffset, int normalOffset);

		void writeNewMaterial(const QString& name);
		void writeAmbientColor(qreal r, qreal g, qreal b);
		void writeDiffuseColor(qreal r, qreal g, qreal b);
		void writeSpecularColor(qreal r, qreal g, qreal b);
	private:
		QTextStream _stream;
	};

	class ObjWriter {
	public:
		void write(const ObjectCollection& objectCollection, const QString& fileName);
		void writeMaterials(const QList<Material>& materials, const QString& fileName);
	};
}

#endif // OBJWRITER_H
