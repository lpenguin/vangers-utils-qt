#ifndef MYORBITCAMERACONTROLLER_H
#define MYORBITCAMERACONTROLLER_H

#include <Qt3DExtras/QAbstractCameraController>

#include <QVector3D>


class MyOrbitCameraController : public Qt3DExtras::QAbstractCameraController
{
	Q_OBJECT
	Q_PROPERTY(qreal zoomLimit READ zoomLimit WRITE setZoomLimit NOTIFY zoomLimitChanged)
	Q_PROPERTY(qreal sensitivity READ sensitivity WRITE setSensitivity NOTIFY sensitivityChanged)
	Q_PROPERTY(QVector3D upVector READ upVector WRITE setUpVector NOTIFY upVectorChanged)
public:
	MyOrbitCameraController(Qt3DCore::QNode *parent = nullptr);
	inline qreal zoomLimit() const
	{
		return m_zoomLimit;
	}

	inline qreal sensitivity() const
	{
		return m_sensitivity;
	}

	inline QVector3D upVector() const
	{
		return m_upVector;
	}

public slots:
	void setZoomLimit(qreal zoomLimit);
	void setSensitivity(qreal sensitivity);

	void setUpVector(QVector3D upVector);

signals:
	void zoomLimitChanged(qreal zoomLimit);
	void sensitivityChanged(qreal sensitivity);
	void upVectorChanged(QVector3D upVector);

private:
	virtual void moveCamera(const InputState &state, float dt);
	qreal m_zoomLimit;
	qreal m_sensitivity;
	QVector3D m_upVector;
};





#endif // MYORBITCAMERACONTROLLER_H
