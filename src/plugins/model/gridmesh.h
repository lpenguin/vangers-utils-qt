#ifndef GRIDMESH_H
#define GRIDMESH_H

#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>


class GridGeometry : public Qt3DRender::QGeometry
{
	Q_OBJECT
	Q_PROPERTY(float lineStep READ lineStep WRITE setLineStep)
	Q_PROPERTY(int lineCount READ lineCount WRITE setLineCount)

public:
	GridGeometry(QNode* parent = nullptr);

	inline float lineStep() const
	{
		return m_lineStep;
	}
	inline int lineCount() const
	{
		return m_lineCount;
	}

public slots:
	void setLineStep(float lineStep);
	void setLineCount(int lineCount);

private:
	void updateGeometry();
	QPair<QByteArray, QByteArray> generateBufferData();

	float m_lineStep;
	int m_lineCount;

	Qt3DRender::QAttribute* m_positionAttribute;
	Qt3DRender::QAttribute* m_indexAttribute;
	Qt3DRender::QBuffer* m_vertexBuffer;
	Qt3DRender::QBuffer* m_indexBuffer;

};

class GridMesh : public Qt3DRender::QGeometryRenderer
{
	Q_OBJECT
public:
	GridMesh(Qt3DCore::QNode* parent = nullptr);

	GridGeometry* gridGeometry() const {
		return dynamic_cast<GridGeometry*>(geometry());
	}
};
#endif // GRIDMESH_H
