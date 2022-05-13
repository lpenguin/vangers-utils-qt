#ifndef OBJWRITER_H
#define OBJWRITER_H

#include <QIODevice>
#include <QTextStream>

namespace model::obj {
	struct ObjFaceIndex {
		int vertexIndex;
		int normalIndex;
		int textureCoordIndex;
	};

	class ObjWriter
	{
	public:
		ObjWriter(QIODevice* ioDevice);
		void writeStartObject(const QString& name);
		void writeStartGroup(const QString& name);
		void writeUseMaterial(const QString& name);
		void writeVertex(qreal x, qreal y, qreal z);
		void writeNormal(qreal x, qreal y, qreal z);
		void writeFace(const QList<ObjFaceIndex>& indices);

		void writeNewMaterial(const QString& name);
		void writeAmbientColor(qreal r, qreal g, qreal b);
		void writeDiffuseColor(qreal r, qreal g, qreal b);
	private:
		QTextStream _stream;
		int _vertexOffset;
		int _normalOffset;
	};
}

#endif // OBJWRITER_H
