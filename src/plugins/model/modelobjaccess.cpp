#include "modelobjaccess.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QtMath>
#include "objwriter.h"

using namespace vangers;

template<typename T>
model::Vector3<qreal> normalize(const model::Vector3<T>& v) {
	T length = qSqrt((qreal)(v.x * v.x + v.y * v.y + v.z * v.z));
	return {
		.x = (qreal)v.x / length,
		.y = (qreal)v.y / length,
		.z = (qreal)v.z / length,
	};
}

void write_c3d(const QString& objectName, const model::C3D& c3d, model::obj::ObjWriter& writer){
	writer.writeStartObject(objectName);

	for(const model::Vertex& vertex: c3d.vectices){
		writer.writeVertex(vertex.tf.x, vertex.tf.y, vertex.tf.z);
	}

	for(const model::PolygonNormal& n: c3d.normals) {
		model::Vector3<qreal> normal = normalize(n.normal);
		writer.writeNormal(normal.x, normal.y, normal.z);
	}

	for(const model::Polygon& p: c3d.polygons){
		QList<model::obj::ObjFaceIndex> indices;

		for(const model::PolygonIndex& i: p.indices){
			indices.append({
							   .vertexIndex = i.vertInd + 1,
							   .normalIndex = i.normInd + 1
						   });
		}
		writer.writeFace(indices);
	}
}

void ModelObjAccess::write(const model::M3D& model, const QString& outputPath)
{
//	QDir outputDir(outputPath);
//	QString objFilename = outputDir.absoluteFilePath("model.obj");

	QFile obj(outputPath);
	if(!obj.open(QFile::WriteOnly | QIODevice::Text)){
		return;
	}
	model::obj::ObjWriter writer(&obj);
	write_c3d("body", model.body, writer);
	write_c3d("bound", model.bound, writer);
	for(size_t i = 0; i < model.debris.size(); i++){
		const model::C3D& debree = model.debris[i];
		write_c3d(QString("debree%1").arg(i), debree, writer);
	}

	for(size_t i = 0; i < model.boundDebris.size(); i++){
		const model::C3D& boundDebree = model.boundDebris[i];
		write_c3d(QString("boundDebree%1").arg(i), boundDebree, writer);
	}

	for(size_t i = 0; i < model.wheels.size(); i++){
		const model::Wheel& wheel = model.wheels[i];
		for(const model::C3D& wheelModel: wheel.model){
			write_c3d(QString("wheel%1").arg(i), wheelModel, writer);
		}
	}

	obj.close();
}
