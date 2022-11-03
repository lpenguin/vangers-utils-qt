#include "c3drenderer.h"
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <vangers/core/palette/palette.h>
#include <QColor>
#include <QVector3D>
#include <m3d/colortable.h>

using namespace m3d;
using namespace vangers::plugins::model;
using namespace vangers::core::palette;

using QAttribute = Qt3DRender::QAttribute;

struct DrawVBOData {
	QVector3D position;
	QVector3D normal;
	QVector3D color;
};


C3DMesh::C3DMesh(const C3D& c3d, Qt3DCore::QNode* parent)
	: QGeometryRenderer(parent)
{
	C3DGeometry* geometry = new C3DGeometry(c3d, this);
	QGeometryRenderer::setGeometry(geometry);
}

C3DGeometry::C3DGeometry(const C3D& c3d, Qt3DCore::QNode* parent)
	: Qt3DRender::QGeometry(parent)
{
	init(c3d);
}

void C3DGeometry::init(const C3D& c3d)
{
	Qt3DRender::QAttribute* m_positionAttribute = new Qt3DRender::QAttribute(this);
	Qt3DRender::QAttribute* m_normalAttribute = new Qt3DRender::QAttribute(this);
	Qt3DRender::QAttribute* m_colorAttribute = new Qt3DRender::QAttribute(this);
	Qt3DRender::QAttribute* m_indexAttribute = new QAttribute(this);

	Qt3DRender::QBuffer* m_vertexBuffer = new Qt3DRender::QBuffer(this);
	Qt3DRender::QBuffer* m_indexBuffer = new Qt3DRender::QBuffer(this);

	const int stride = sizeof (DrawVBOData);
	int nVerts = 0;
	for(const Polygon& p: c3d.polygons){
		nVerts += p.num;
	}

	int nInds = 0;
	for(const Polygon& p: c3d.polygons){
		nInds += p.num == 3 ? 3 : 6;
	}


	m_positionAttribute->setName(QAttribute::defaultPositionAttributeName());
	m_positionAttribute->setVertexBaseType(QAttribute::Float);
	m_positionAttribute->setVertexSize(3);
	m_positionAttribute->setAttributeType(QAttribute::VertexAttribute);
	m_positionAttribute->setBuffer(m_vertexBuffer);
	m_positionAttribute->setByteStride(stride);
	m_positionAttribute->setByteOffset(offsetof(DrawVBOData, position));
	m_positionAttribute->setCount(nVerts);

	m_normalAttribute->setName(QAttribute::defaultNormalAttributeName());
	m_normalAttribute->setVertexBaseType(QAttribute::Float);
	m_normalAttribute->setVertexSize(3);
	m_normalAttribute->setAttributeType(QAttribute::VertexAttribute);
	m_normalAttribute->setBuffer(m_vertexBuffer);
	m_normalAttribute->setByteStride(stride);
	m_normalAttribute->setByteOffset(offsetof(DrawVBOData, normal));
	m_normalAttribute->setCount(nVerts);

	m_colorAttribute->setName(QAttribute::defaultColorAttributeName());
	m_colorAttribute->setVertexBaseType(QAttribute::Float);
	m_colorAttribute->setVertexSize(3);
	m_colorAttribute->setAttributeType(QAttribute::VertexAttribute);
	m_colorAttribute->setBuffer(m_vertexBuffer);
	m_colorAttribute->setByteStride(stride);
	m_colorAttribute->setByteOffset(offsetof(DrawVBOData, color));
	m_colorAttribute->setCount(nVerts);

	m_indexAttribute->setAttributeType(QAttribute::IndexAttribute);
	m_indexAttribute->setVertexBaseType(QAttribute::UnsignedShort);
	m_indexAttribute->setBuffer(m_indexBuffer);
	m_indexAttribute->setCount(nInds);

	m_vertexBuffer->setData(createC3DVertexData(c3d));
	m_indexBuffer->setData(createC3DIndexData(c3d));

	this->addAttribute(m_positionAttribute);
	this->addAttribute(m_normalAttribute);
	this->addAttribute(m_colorAttribute);
	this->addAttribute(m_indexAttribute);
}

QByteArray vangers::plugins::model::createC3DVertexData(const C3D& c3d)
{
	Palette pal = Palette::read("objects.pal");

	QMap<int32_t, QColor> colorMap;

	int nVerts = 0;
	for(const Polygon& p: c3d.polygons){
		nVerts += p.num;
	}

	const quint32 stride = sizeof (DrawVBOData);
	QByteArray bufferBytes;
	bufferBytes.resize(stride * nVerts);
	DrawVBOData* fptr = reinterpret_cast<DrawVBOData*>(bufferBytes.data());

	for(const Polygon& polygon: c3d.polygons){
//		if(polygon.num != 3){
//			return {};
//		}

		uint32_t colorId = polygon.colorId;
		if(colorId == 1) {
			// TODO: take M3D bodyColor/bodyOffset
			colorId = (uint32_t)ColorId::BODY_GREEN;
		}
		if(!colorMap.contains(colorId)){
			uint8_t color_index = get_color_index(colorId);
			colorMap[colorId] = pal[color_index];
		}
		QColor c = colorMap[colorId];

		QVector3D colV{(float)c.redF(), (float)c.greenF(), (float)c.blueF()};


//		for(int i = polygon.indices.size() - 1; i >= 0; i--) {
		for(int i = 0; i < polygon.indices.size(); i++) {
			const PolygonIndex& ind = polygon.indices[i];
			DrawVBOData* data = fptr++;

			Vector3F32 tf = c3d.vectices[ind.vertInd].tf;
			QVector3D posV = {tf.x, tf.y, tf.z};


			data->position = posV;
			data->color = colV;

			Vector3I8 norm = c3d.normals[ind.normInd].normal;
			QVector3D normal = {(float)norm.x, (float)norm.y, (float)norm.z};
			normal.normalize();
			data->normal = normal;
		}
	}

	return bufferBytes;
}

QByteArray vangers::plugins::model::createC3DIndexData(const C3D& c3d)
{
	int nInds = 0;
	for(const Polygon& p: c3d.polygons){
		if(p.num == 3){
			nInds += 3;
		} else if(p.num == 4){
			nInds += 6;
		} else {
			qDebug() << "Invalid polygon size: " << p.num;
			return {};
		}
	}

	const quint32 stride = sizeof (uint16_t);
	QByteArray bufferBytes;
	bufferBytes.resize(stride * nInds);
	uint16_t* fptr = reinterpret_cast<uint16_t*>(bufferBytes.data());
//	QList<quint32> indices;


	uint16_t iVertex = 0;
	for(const Polygon& polygon: c3d.polygons){
		if(polygon.num == 3){
			(*fptr++) = iVertex + 2;
			(*fptr++) = iVertex + 1;
			(*fptr++) = iVertex + 0;
			iVertex += 3;
		} else if(polygon.num == 4){
			(*fptr++) = iVertex + 2;
			(*fptr++) = iVertex + 1;
			(*fptr++) = iVertex + 0;
			(*fptr++) = iVertex + 3;
			(*fptr++) = iVertex + 2;
			(*fptr++) = iVertex + 0;
			iVertex += 4;
		}


	}

	return bufferBytes;
}
