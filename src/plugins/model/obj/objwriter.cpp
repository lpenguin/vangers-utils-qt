#include "objwriter.h"
#include <QFile>
#include <QStringBuilder>

using namespace vangers::model::obj;

ObjStreamWriter::ObjStreamWriter(QIODevice* ioDevice)
	: _stream(ioDevice)
{

}

void ObjStreamWriter::writeStartObject(const QString& name)
{
	_stream << QString("o %1\n").arg(name);
}

void ObjStreamWriter::writeStartGroup(const QString& name)
{
	_stream << QString("g %1\n").arg(name);
}

void ObjStreamWriter::writeUseMaterial(const QString& name)
{
	_stream << QString("usemtl %1\n").arg(name);
}

void ObjStreamWriter::writeUseMaterialLib(const QString& fileName)
{
	_stream << QString("mtllib %1\n").arg(fileName);
}

void ObjStreamWriter::writeVertex(qreal x, qreal y, qreal z)
{
	_stream << QString("v %1 %2 %3\n").arg(x).arg(y).arg(z);
}

void ObjStreamWriter::writeNormal(qreal x, qreal y, qreal z)
{
	_stream << QString("vn %1 %2 %3\n").arg(x).arg(y).arg(z);
}

void ObjStreamWriter::writeFace(const QList<FaceIndex>& indices, int vertexOffset, int normalOffset)
{
	QStringList faces;
	for(const FaceIndex& index: indices){
		faces << QString("%1/%2/%3").arg(
					 index.vertexIndex.hasValue() ? QString::number(index.vertexIndex + 1 + vertexOffset) : "",
					 index.textureCoordIndex.hasValue() ? QString::number(index.textureCoordIndex + 1) : "",
					 index.normalIndex.hasValue() ? QString::number(index.normalIndex + 1 + normalOffset)  : ""
				);
	}

	_stream << QString("f %1\n").arg(faces.join(' '));
}

void ObjStreamWriter::writeNewMaterial(const QString& name)
{
	_stream << QString("newmtl %1\n").arg(name);
}

void ObjStreamWriter::writeAmbientColor(qreal r, qreal g, qreal b)
{
	_stream << QString("Ka %1 %2 %3\n").arg(r).arg(g).arg(b);
}

void ObjStreamWriter::writeDiffuseColor(qreal r, qreal g, qreal b)
{
	_stream << QString("Kd %1 %2 %3\n").arg(r).arg(g).arg(b);
}

void ObjStreamWriter::writeSpecularColor(qreal r, qreal g, qreal b)
{
	_stream << QString("Ks %1 %2 %3\n").arg(r).arg(g).arg(b);
}

void ObjWriter::write(const ObjectCollection& objectCollection, const QString& fileName)
{
	QFile f(fileName);
	if(!f.open(QFile::WriteOnly | QFile::Text)) return;

	ObjStreamWriter writer(&f);
	for(const auto& lib: objectCollection.useMaterialLibraries){
		writer.writeUseMaterialLib(lib);
	}

	int32_t vertexOffset = 0;
	int32_t normalOffset = 0;

	for(const Object& object: objectCollection.objects){
		writer.writeStartObject(object.name);
		for(const Vector3F64& v: object.vertices){
			writer.writeVertex(v.x, v.y, v.z);
		}

		for(const Vector3F64& v: object.normals){
			writer.writeNormal(v.x, v.y, v.z);
		}
		for(const Group& group: object.groups){
			if(!group.name.isNull()){
				writer.writeStartGroup(group.name);
			}

			if(!group.material.isNull()){
				writer.writeUseMaterial(group.material);
			}

			for(const Face& face: group.faces){
				writer.writeFace(face.indices, vertexOffset, normalOffset);
			}
		}

		vertexOffset += object.vertices.size();
		normalOffset += object.normals.size();
	}



}

void ObjWriter::writeMaterials(const QList<Material>& materials, const QString& fileName)
{
	QFile f(fileName);
	if(!f.open(QFile::WriteOnly | QFile::Text)) return;

	ObjStreamWriter writer(&f);

	for(const Material& m: materials){
		writer.writeNewMaterial(m.name);
		writer.writeAmbientColor(m.ambientColor.x, m.ambientColor.y, m.ambientColor.z);
		writer.writeDiffuseColor(m.diffuseColor.x, m.diffuseColor.y, m.diffuseColor.z);
		writer.writeSpecularColor(m.specularColor.x, m.specularColor.y, m.specularColor.z);
	}
}


