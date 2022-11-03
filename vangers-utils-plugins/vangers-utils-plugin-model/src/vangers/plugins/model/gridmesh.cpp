#include "gridmesh.h"

#include <QVector3D>

using namespace Qt3DCore;
using namespace Qt3DRender;

GridMesh::GridMesh(Qt3DCore::QNode* parent)
	: Qt3DRender::QGeometryRenderer(parent)
{
	setGeometry(new GridGeometry(this));
	setPrimitiveType(QGeometryRenderer::Lines);
}

GridGeometry::GridGeometry(Qt3DCore::QNode* parent)
	: QGeometry(parent)
	, m_lineStep(1.0f)
	, m_lineCount(3)
{
	m_vertexBuffer = new QBuffer(this);
	m_indexBuffer = new QBuffer(this);

	m_positionAttribute = new QAttribute(this);
	m_positionAttribute->setName(QAttribute::defaultPositionAttributeName());
	m_positionAttribute->setVertexBaseType(QAttribute::Float);
	m_positionAttribute->setVertexSize(3);
	m_positionAttribute->setAttributeType(QAttribute::VertexAttribute);
	m_positionAttribute->setBuffer(m_vertexBuffer);
	m_positionAttribute->setByteStride(3 * sizeof (QVector3D));

	m_indexAttribute = new QAttribute(this);
	m_indexAttribute->setAttributeType(QAttribute::IndexAttribute);
	m_indexAttribute->setVertexBaseType(QAttribute::UnsignedShort);
	m_indexAttribute->setBuffer(m_indexBuffer);

	addAttribute(m_positionAttribute);
	addAttribute(m_indexAttribute);

	updateGeometry();
}

void GridGeometry::setLineStep(float lineStep)
{
	m_lineStep = lineStep;
	updateGeometry();
}

void GridGeometry::setLineCount(int lineCount)
{
	m_lineCount = lineCount;
	updateGeometry();
}

void GridGeometry::updateGeometry()
{
	int nVerts = m_lineCount * 2;
	QPair<QByteArray, QByteArray> buffers = generateBufferData();

	m_positionAttribute->setCount(nVerts);
	m_vertexBuffer->setData(buffers.first);

	m_indexAttribute->setCount(nVerts);
	m_indexBuffer->setData(buffers.second);
}

QPair<QByteArray, QByteArray> GridGeometry::generateBufferData()
{
	QByteArray vertexBuffer;
	QByteArray indexBuffer;
	int nVerts = m_lineCount * 2;

	int vertexStride = sizeof (QVector3D);
	int indexStride = sizeof (quint16);

	float bound = (m_lineCount) * m_lineStep * 2;
	bound  = 10;

	vertexBuffer.resize(vertexStride * nVerts);
	indexBuffer.resize(indexStride * nVerts);

	QVector3D* vertices = reinterpret_cast<QVector3D*>(vertexBuffer.data());
	quint16* indices = reinterpret_cast<quint16*>(indexBuffer.data());

	int index = 0;
	for(int i = 0; i < m_lineCount; i++){
		vertices[indices[index++] = index] = QVector3D(-bound/2.0f, 0, i * m_lineStep);
		vertices[indices[index++] = index] = QVector3D(bound/2.0f, 0, i * m_lineStep);
//		vertices[indices[index++] = index] = QVector3D(bound/2.0f, 0, i * m_lineStep);
//		vertices[indices[index] = index++] = QVector3D(-bound/2.0f, 0, -i * m_lineStep);
//		vertices[indices[index] = index++] = QVector3D(bound/2.0f, 0, -i * m_lineStep);
	}

//	for(int i = 0; i < m_lineCount; i++){
//		vertices[indices[index] = index++] = QVector3D(i * m_lineStep, 0, -bound/2.0f);
//		vertices[indices[index] = index++] = QVector3D(i * m_lineStep, 0, bound/2.0f);
//		vertices[indices[index] = index++] = QVector3D(-i * m_lineStep, 0, -bound/2.0f);
//		vertices[indices[index] = index++] = QVector3D(-i * m_lineStep, 0, bound/2.0f);
//	}

	return {vertexBuffer, indexBuffer};
}
