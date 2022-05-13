#ifndef SCENECONTROLLER_H
#define SCENECONTROLLER_H

#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/QTransform>

#include "model.h"

class SceneController: QObject
{
Q_OBJECT
public:
	SceneController(Qt3DExtras::Qt3DWindow * view, QObject* parent = nullptr);
	void setM3D(const QSharedPointer<model::M3D>& m3d);
private:
	void init();
	Qt3DExtras::Qt3DWindow *_view;
	Qt3DCore::QEntity *_rootEntity;
	Qt3DCore::QEntity *_modelsEntity;
};

#endif // SCENECONTROLLER_H
