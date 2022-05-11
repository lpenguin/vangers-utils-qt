#ifndef MYORBITCAMERACONTROLLER_H
#define MYORBITCAMERACONTROLLER_H

#include <Qt3DExtras/QAbstractCameraController>

class MyOrbitCameraController : public Qt3DExtras::QAbstractCameraController
{
	Q_OBJECT
	Q_PROPERTY(qreal zoomLimit READ zoomLimit WRITE setZoomLimit NOTIFY zoomLimitChanged)
public:
	MyOrbitCameraController(Qt3DCore::QNode *parent = nullptr);
	qreal zoomLimit() const
	{
		return m_zoomLimit;
	}

public slots:
	void setZoomLimit(qreal zoomLimit)
	{
		qWarning("Floating point comparison needs context sanity check");
		if (qFuzzyCompare(m_zoomLimit, zoomLimit))
			return;

		m_zoomLimit = zoomLimit;
		emit zoomLimitChanged(m_zoomLimit);
	}

signals:
	void zoomLimitChanged(qreal zoomLimit);

private:
	virtual void moveCamera(const InputState &state, float dt);
	qreal m_zoomLimit;
};

#endif // MYORBITCAMERACONTROLLER_H
