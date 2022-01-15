#include "m3dadapter.h"
#include <image/palette.h>
#include <QColor>

M3DAdapter::M3DAdapter()
{

}

bool M3DAdapter::adapt(const model::M3D &m3d, QVector<DrawVBOData> &vertices, QVector<uint32_t> &indices)
{
//	vangers::Palette pal = vangers::Palette::read("fostral1.pal");
//	qDebug() << "pal size" << pal.size();
	QMap<int32_t, QColor> colorMap;

//	for(const model::Vertex& vertex: m3d.body.vectices){
//		model::Vector3F32 tf = vertex.tf;
//		DrawVBOData data {
//			.position = {tf.x, tf.z, tf.y},
//			.color = {0, 0, 0},
//		};
//		data.position = data.position / 16.0;

//		vertices.append(data);
//	}
	int iVertex = 0;
	for(const model::Polygon& polygon: m3d.body.polygons){
		if(polygon.num != 3){
			return false;
		}

		if(!colorMap.contains(polygon.colorId)){
			colorMap[polygon.colorId] = QColor(qrand()%255, qrand()%255, qrand()%255);
		}
		QColor c = colorMap[polygon.colorId];

//		QVector3D colV{c.redF(), c.greenF(), c.blackF()};
		QVector3D colV{(float)c.redF(), (float)c.greenF(), (float)c.blueF()};


		qDebug() <<  polygon.colorId << polygon.colorShift << colV;


		for(const model::PolygonIndex& ind: polygon.indices){
			indices.append(iVertex);

			DrawVBOData data{};
			model::Vector3F32 tf = m3d.body.vectices[ind.vertInd].tf;
			QVector3D posV = {tf.x, tf.z, tf.y};
			posV /= 16.0;

			data.position = posV;
			data.color = colV;

			model::Vector3I8 norm = m3d.body.normals[ind.normInd].normal;
			QVector3D normalV = {(float)norm.x, (float)norm.z, (float)norm.y};
			normalV /= 16.0;
			data.normal = normalV;

			vertices.append(data);
			iVertex ++;

		}


	}

	return true;
}
