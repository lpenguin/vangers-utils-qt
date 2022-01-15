#ifndef DRAWDATA_H
#define DRAWDATA_H

#include <Qt3DCore/QNode>
#include <Qt3DRender/QBuffer>
#include <QVector3D>


struct DrawVBOData {
	QVector3D position;
	QVector3D normal;
	QVector3D color;
};

class DrawData : public Qt3DCore::QNode {
	Q_OBJECT
	Q_PROPERTY(Qt3DRender::QBuffer *buffer READ buffer NOTIFY bufferChanged)
	Q_PROPERTY(int count READ count NOTIFY countChanged)
//	Q_PROPERTY(int stride READ stride CONSTANT)

public:
	explicit DrawData (Qt3DCore::QNode *parent = 0);
	Qt3DRender::QBuffer *buffer();


//	template<typename T>
//	void setData(const QVector<T> &datas);

//	template<>
	void setData(const QVector<DrawVBOData> &datas)
	{
		QByteArray ba;
		ba.resize(datas.size() * 9 * sizeof(float));
		const size_t step = 9;
		float *datasV = reinterpret_cast<float*>(ba.data());
		for (int i = 0; i < datas.size(); i++) {
			datasV[i * step + 0] = datas[i].position.x();
			datasV[i * step + 1] = datas[i].position.y();
			datasV[i * step + 2] = datas[i].position.z();
			datasV[i * step + 3] = datas[i].normal.x();
			datasV[i * step + 4] = datas[i].normal.y();
			datasV[i * step + 5] = datas[i].normal.z();
			datasV[i * step + 6] = datas[i].color.x();
			datasV[i * step + 7] = datas[i].color.y();
			datasV[i * step + 8] = datas[i].color.z();
		}
		m_buffer->setData(ba);
		m_count = datas.count();

		emit countChanged(m_count);
		emit bufferChanged(m_buffer.data());
	}

	void setData(const QVector<uint32_t> &datas)
	{
		QByteArray ba;
		ba.resize(datas.size() * sizeof(uint32_t));
		uint32_t *datasV = reinterpret_cast<uint32_t*>(ba.data());
		for (int i = 0; i < datas.size(); i++) {
			datasV[i] = datas[i];
		}
		m_buffer->setData(ba);
		m_count = datas.count();

		emit countChanged(m_count);
		emit bufferChanged(m_buffer.data());
	}


	int count() const;


signals:
	void countChanged(int count);
	void bufferChanged(Qt3DRender::QBuffer *buffer);

public slots:

private:
	QScopedPointer<Qt3DRender::QBuffer> m_buffer;
	int m_count = 0;
};


#endif // DRAWDATA_H
