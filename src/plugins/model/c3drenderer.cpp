#include "c3drenderer.h"
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <image/palette.h>
#include <QColor>
#include <QVector3D>
#include "colortable.h"

using QAttribute = Qt3DRender::QAttribute;

struct DrawVBOData {
	QVector3D position;
	QVector3D normal;
	QVector3D color;
};


C3DMesh::C3DMesh(const QSharedPointer<model::C3D>& c3d, Qt3DCore::QNode* parent)
	: QGeometryRenderer(parent)
{
	C3DGeometry* geometry = new C3DGeometry(c3d, this);
	QGeometryRenderer::setGeometry(geometry);
}

C3DGeometry::C3DGeometry(const QSharedPointer<model::C3D>& c3d, Qt3DCore::QNode* parent)
	: Qt3DRender::QGeometry(parent)
	, _c3d(c3d)
{
	init();
}

void C3DGeometry::init()
{
	Qt3DRender::QAttribute* m_positionAttribute = new Qt3DRender::QAttribute(this);
	Qt3DRender::QAttribute* m_normalAttribute = new Qt3DRender::QAttribute(this);
	Qt3DRender::QAttribute* m_colorAttribute = new Qt3DRender::QAttribute(this);
	Qt3DRender::QAttribute* m_indexAttribute = new QAttribute(this);

	Qt3DRender::QBuffer* m_vertexBuffer = new Qt3DRender::QBuffer(this);
	Qt3DRender::QBuffer* m_indexBuffer = new Qt3DRender::QBuffer(this);

	const int stride = sizeof (DrawVBOData);
	int nVerts = 3 * _c3d->polygons.size();

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
	m_indexAttribute->setCount(nVerts);

	m_vertexBuffer->setDataGenerator(QSharedPointer<C3DVertexBufferFunctor>::create(_c3d));
	m_indexBuffer->setDataGenerator(QSharedPointer<C3DIndexBufferFunctor>::create(_c3d));

	this->addAttribute(m_positionAttribute);
	this->addAttribute(m_normalAttribute);
	this->addAttribute(m_colorAttribute);
	this->addAttribute(m_indexAttribute);
}

QByteArray createC3DVertexData(const model::C3D& c3d)
{
	vangers::Palette pal = vangers::Palette::read("objects.pal");

	QMap<int32_t, QColor> colorMap;

	int nVerts = 3 * c3d.polygons.size();

	const quint32 stride = sizeof (DrawVBOData);
	QByteArray bufferBytes;
	bufferBytes.resize(stride * nVerts);
	DrawVBOData* fptr = reinterpret_cast<DrawVBOData*>(bufferBytes.data());

	for(const model::Polygon& polygon: c3d.polygons){
		if(polygon.num != 3){
			return {};
		}

		uint32_t colorId = polygon.colorId;
		if(colorId == 1) {
			// TODO: take M3D bodyColor/bodyOffset
			colorId = (uint32_t)COLORS_IDS::BODY_GREEN;
		}
		if(!colorMap.contains(colorId)){
			uint8_t color_index = get_color_index(colorId);
			colorMap[colorId] = pal[color_index];
		}
		QColor c = colorMap[colorId];

		QVector3D colV{(float)c.redF(), (float)c.greenF(), (float)c.blueF()};


		for(int i = polygon.indices.size() - 1; i >= 0; i--) {
			const model::PolygonIndex& ind = polygon.indices[i];
			DrawVBOData* data = fptr++;

			model::Vector3F32 tf = c3d.vectices[ind.vertInd].tf;
			QVector3D posV = {tf.x, tf.y, tf.z};


			data->position = posV;
			data->color = colV;

			model::Vector3I8 norm = c3d.normals[ind.normInd].normal;
			QVector3D normal = {(float)norm.x, (float)norm.y, (float)norm.z};
			data->normal = normal.normalized();
		}
	}

	return bufferBytes;
}

QByteArray createC3DIndexData(const model::C3D& c3d)
{
	int nVerts = 3 * c3d.polygons.size();
	const quint32 stride = sizeof (uint16_t);
	QByteArray bufferBytes;
	bufferBytes.resize(stride * nVerts);
	uint16_t* fptr = reinterpret_cast<uint16_t*>(bufferBytes.data());

	uint16_t iVertex = 0;
	for(const model::Polygon& polygon: c3d.polygons){
		if(polygon.num != 3){
			return {};
		}

		for(ulong i = 0; i < polygon.indices.size(); i++){
			(*fptr++) = iVertex++;
		}
	}

	return bufferBytes;
}
