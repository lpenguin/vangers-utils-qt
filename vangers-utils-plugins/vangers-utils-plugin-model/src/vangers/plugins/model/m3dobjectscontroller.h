#ifndef M3DOBJECTSCONTROLLER_H
#define M3DOBJECTSCONTROLLER_H

#include <m3d/m3d.h>
#include <QTreeWidget>

#include "scenecontroller.h"


namespace vangers::plugins::model{
	class M3DObjectsController : public QObject
	{
		Q_OBJECT
	public:
		M3DObjectsController(QTreeWidget* treeWidget,
		                     SceneController* sceneController,
		                     QObject* parent = nullptr);
		void setM3D(const m3d::M3D& m3d);

	private slots:
		void onItemClicked(QTreeWidgetItem* item, int column);

	private:
		void initializeTree(const m3d::M3D& m3d);
		QTreeWidget* _treeWidget;
		SceneController* _sceneController;
	};
}


#endif // M3DOBJECTSCONTROLLER_H
