#include "objwriter.h"
#include <QStringBuilder>

using namespace model::obj;

ObjWriter::ObjWriter(QIODevice* ioDevice)
	: _stream(ioDevice)
	, _totalObject({})
	, _curObject({})
{

}

void ObjWriter::writeStartObject(const QString& name)
{
	_totalObject.numVertices += _curObject.numVertices;
	_totalObject.numNormals += _curObject.numNormals;
	_curObject = {};

	_stream << QString("o %1\n").arg(name);
}

void ObjWriter::writeStartGroup(const QString& name)
{
	_stream << QString("g %1\n").arg(name);
}

void ObjWriter::writeUseMaterial(const QString& name)
{
	_stream << QString("usemtl %1\n").arg(name);
}

void ObjWriter::writeVertex(qreal x, qreal y, qreal z)
{
	_stream << QString("v %1 %2 %3\n").arg(x).arg(y).arg(z);
	_curObject.numVertices += 1;
}

void ObjWriter::writeNormal(qreal x, qreal y, qreal z)
{
	_stream << QString("n %1 %2 %3\n").arg(x).arg(y).arg(z);
	_curObject.numNormals += 1;
}

void ObjWriter::writeFace(const QList<ObjFaceIndex>& indices)
{
	QStringList faces;
	for(const ObjFaceIndex& index: indices){
		faces << QString("%1/%2/%3").arg(
					 index.vertexIndex == 0 ? "" : QString::number(index.vertexIndex + _totalObject.numVertices),
					 index.textureCoordIndex == 0 ? "" : QString::number(index.textureCoordIndex),
					 index.normalIndex == 0 ? "" : QString::number(index.normalIndex + _totalObject.numNormals)
				);
	}

	_stream << QString("f %1\n").arg(faces.join(' '));
}

void ObjWriter::writeNewMaterial(const QString& name)
{
	_stream << QString("newmtl %1\n").arg(name);
}

void ObjWriter::writeAmbientColor(qreal r, qreal g, qreal b)
{
	_stream << QString("Ka %1 %2 %3\n").arg(r).arg(g).arg(b);
}

void ObjWriter::writeDiffuseColor(qreal r, qreal g, qreal b)
{
	_stream << QString("Kd %1 %2 %3\n").arg(r).arg(g).arg(b);
}
