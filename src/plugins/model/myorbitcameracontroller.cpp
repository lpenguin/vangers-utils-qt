#include "myorbitcameracontroller.h"
#include <Qt3DRender/QCamera>
#include <Qt3DInput/QMouseDevice>

MyOrbitCameraController::MyOrbitCameraController(Qt3DCore::QNode* parent)
	: Qt3DExtras::QAbstractCameraController(parent)
	, m_zoomLimit(2.0)
	, m_sensitivity(0.1f)
	, m_upVector(0.0f, 0.0f, 1.0f)
{
	mouseDevice()->setSensitivity(m_sensitivity);
}

inline float clampInputs(float input1, float input2)
{
	float axisValue = input1 + input2;
	return (axisValue < -1) ? -1 : (axisValue > 1) ? 1 : axisValue;
}

inline float zoomDistance(QVector3D firstPoint, QVector3D secondPoint)
{
	return (secondPoint - firstPoint).lengthSquared();
}

void MyOrbitCameraController::moveCamera(const Qt3DExtras::QAbstractCameraController::InputState& state, float dt)
{
	Qt3DRender::QCamera *theCamera = camera();

	if (state.leftMouseButtonActive) {
		// Orbit
//		dt = 0.03f;
		theCamera->panAboutViewCenter((-state.rxAxisValue * lookSpeed()) * dt, m_upVector);
		theCamera->tiltAboutViewCenter((-state.ryAxisValue * lookSpeed()) * dt);
	}

	if(state.tzAxisValue != 0){
		if (zoomDistance(theCamera->position(), theCamera->viewCenter()) > m_zoomLimit * m_zoomLimit) {
			// Dolly up to limit
			theCamera->translate(QVector3D(0, 0, state.tzAxisValue), theCamera->DontTranslateViewCenter);
		} else {
			theCamera->translate(QVector3D(0, 0, -0.5), theCamera->DontTranslateViewCenter);
		}
	}
}

void MyOrbitCameraController::setSensitivity(qreal sensitivity)
{
	qWarning("Floating point comparison needs context sanity check");
	if (qFuzzyCompare(m_sensitivity, sensitivity))
		return;

	m_sensitivity = sensitivity;
	mouseDevice()->setSensitivity(m_sensitivity);
	emit sensitivityChanged(m_sensitivity);
}

void MyOrbitCameraController::setUpVector(QVector3D upVector)
{
	if (m_upVector == upVector)
		return;

	m_upVector = upVector;
	emit upVectorChanged(m_upVector);
}

void MyOrbitCameraController::setZoomLimit(qreal zoomLimit)
{
	qWarning("Floating point comparison needs context sanity check");
	if (qFuzzyCompare(m_zoomLimit, zoomLimit))
		return;

	m_zoomLimit = zoomLimit;
	emit zoomLimitChanged(m_zoomLimit);
}
