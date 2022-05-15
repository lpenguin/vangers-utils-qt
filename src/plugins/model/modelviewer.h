#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <variant>
#include <plugins/resourceviewer.h>
#include "ui_modelviewer.h"
#include "scenecontroller.h"
#include "m3dobjectscontroller.h"

#include <plugins/model/objimport/objimportsettingswidget.h>
#include <plugins/model/objimport/objimportcontroller.h>

namespace vangers::model::view {
	using namespace vangers::model::objimport;

	class ModelViewer : public ResourceViewer
	{
		Q_OBJECT
	public:

		ModelViewer(ResourceViewerPlugin* plugin, QWidget* parent = nullptr);

		bool importResource(const QString &filePath, const ResourceType &resourceType);
		void exportResource(const QString &filePath, const ResourceType &resourceType);

		QString currentFile() const {
			return _currentFile;
		}

	private:
		QString _currentFile;
		Ui::ModelViewer* _ui;
		QWidget* _widget3d;
		SceneController* _sceneController;
		M3DObjectsController* _objectsController;
		ObjImportSettingsWidget* _objImportWidget;
		ObjImportController* _objImportController;
		QSharedPointer<M3D> _model;
		int _a3dModelIndex;

		void showA3dModel(A3D& a3d);
		void setupModel();

		bool importM3D(const QString &filePath);
		bool importObj(const QString &filePath);

		void exportM3D(const QString& filePath);
		void exportObj(const QString &filePath);
	private slots:
		void onTreeItemClicked(QTreeWidgetItem *item, int column);
		void onTreeContextMenuRequested(const QPoint& pos);
		void onPrevModelClicked();
		void onNextModelClicked();
		void onImportSettingsChanged();
	};
}


#endif // MODELVIEWER_H
