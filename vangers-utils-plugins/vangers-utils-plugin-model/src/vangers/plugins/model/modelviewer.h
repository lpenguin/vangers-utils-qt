#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <variant>

#include <vangers/core/plugin/resourceviewer.h>

#include <vangers/plugins/model/scenecontroller.h>
#include <vangers/plugins/model/m3dobjectscontroller.h>
#include <vangers/plugins/model/objimport/objimportsettingswidget.h>
#include <vangers/plugins/model/objimport/objimportcontroller.h>

#include "ui_modelviewer.h"

namespace vangers::plugins::model {
	class ModelViewer : public vangers::core::plugin::ResourceViewer
	{
		Q_OBJECT
	public:

		ModelViewer(vangers::core::plugin::ResourceViewerPlugin* plugin, QWidget* parent = nullptr);

		bool importResource(const QString &filePath, const vangers::core::plugin::ResourceType &resourceType) override;
		void exportResource(const QString &filePath, const vangers::core::plugin::ResourceType &resourceType) override;

		QString currentFile() const override {
			return _currentFile;
		}
	protected:
		void hideEvent(QHideEvent* event) override;
		void showEvent(QShowEvent* event) override;
	private:
		QString _currentFile;
		Ui::ModelViewer* _ui;
		QWidget* _widget3d;
		Qt3DExtras::Qt3DWindow *_view;
		SceneController* _sceneController;
		M3DObjectsController* _objectsController;
		objimport::ObjImportController* _objImportController;
		QSharedPointer<m3d::M3D> _model;
		int _a3dModelIndex;

		void showA3dModel(m3d::A3D& a3d);
		void setupModel();

		bool importM3D(const QString &filePath);
		void exportM3D(const QString& filePath);

		bool importObj(const QString &filePath);
		void exportObj(const QString &filePath);

//		bool importJson(const QString &filePath);
//		void exportJson(const QString& filePath);
	private slots:
		void onTreeItemClicked(QTreeWidgetItem *item, int column);
		void onTreeContextMenuRequested(const QPoint& pos);
		void onPrevModelClicked();
		void onNextModelClicked();
		void onImportSettingsChanged();
	};
}


#endif // MODELVIEWER_H
