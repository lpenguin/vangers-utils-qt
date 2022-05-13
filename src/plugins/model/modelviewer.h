#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <plugins/resourceviewer.h>
#include "ui_modelviewer.h"
#include "scenecontroller.h"

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
	model::M3D _m3d;
private slots:
	void onTreeItemClicked(QTreeWidgetItem *item, int column);
};

#endif // MODELVIEWER_H
