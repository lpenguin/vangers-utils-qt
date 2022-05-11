#include "myorbitcameracontroller.h"
#include <Qt3DRender/QCamera>

MyOrbitCameraController::MyOrbitCameraController(Qt3DCore::QNode* parent)
	: Qt3DExtras::QAbstractCameraController(parent)
	, m_zoomLimit(2.0)
{

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
	const QVector3D upVector(0.0f, 0.0f, 1.0f);

	Qt3DRender::QCamera *theCamera = camera();

	if (state.rightMouseButtonActive) {
		// Orbit
		theCamera->panAboutViewCenter((state.rxAxisValue * lookSpeed()) * dt, upVector);
		theCamera->tiltAboutViewCenter((state.ryAxisValue * lookSpeed()) * dt);
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
