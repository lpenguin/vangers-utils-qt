#include "drawdata.h"


DrawData::DrawData(Qt3DCore::QNode *parent)
	: Qt3DCore::QNode(parent)
	, m_buffer(new Qt3DRender::QBuffer(this))
	, m_count(0)
{
	m_buffer->setAccessType(Qt3DRender::QBuffer::ReadWrite);
}

Qt3DRender::QBuffer *DrawData::buffer()
{
	qDebug()<< "buffer()"<<m_buffer.data();
	return m_buffer.data();
}



int DrawData::count() const
{
	return m_count;
}

