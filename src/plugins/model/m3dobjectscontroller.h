#ifndef M3DOBJECTSCONTROLLER_H
#define M3DOBJECTSCONTROLLER_H

#include <plugins/model/m3d/m3d.h>
#include "scenecontroller.h"

#include <QTreeWidget>

namespace vangers::model::view {
	using namespace vangers::model::m3d;

	class M3DObjectsController : public QObject
	{
		Q_OBJECT
	public:
		M3DObjectsController(QTreeWidget* treeWidget,
		                     SceneController* sceneController,
		                     QObject* parent = nullptr);
		void setM3D(const M3D& m3d);

	private slots:
		void onItemClicked(QTreeWidgetItem* item, int column);

	private:
		void initializeTree(const M3D& m3d);
		QTreeWidget* _treeWidget;
		SceneController* _sceneController;
	};
}


#endif // M3DOBJECTSCONTROLLER_H
