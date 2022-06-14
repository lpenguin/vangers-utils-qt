#ifndef SCENECONTROLLER_H
#define SCENECONTROLLER_H

#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/QTransform>

#include <plugins/model/m3d/m3d.h>

#include "c3dview.h"
#include "slotview.h"

namespace vangers::model::view {
	using namespace vangers::model::m3d;

	class SceneController : public QObject
	{
	Q_OBJECT
	public:
		SceneController(Qt3DExtras::Qt3DWindow * view, QObject* parent = nullptr);

		void setM3D(const M3D& m3d);

		void setBodyVisible(bool visible);
		void setBoundVisible(bool visible);
		void setDebrisVisible(int index, bool visible);
		void setDebrisBoundVisible(int index, bool visible);
		void setWheelVisible(int index, bool visible);
		void setSlotVisible(int index, bool visible);

		int slotCount();
	public slots:
		void resetView();
	private:
		void init();
		Qt3DExtras::Qt3DWindow *_view;
		Qt3DCore::QEntity *_rootEntity;
		Qt3DCore::QEntity *_modelsEntity;

		C3DView* _bodyView;
		C3DView* _boundView;
		QList<C3DView*> _wheelViews;
		QList<C3DView*> _debrisViews;
		QList<C3DView*> _debrisBoundViews;
		QList<SlotView*> _slotViews;
	};
}



#endif // SCENECONTROLLER_H
