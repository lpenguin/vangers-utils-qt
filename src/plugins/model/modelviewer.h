#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <variant>
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
	std::variant<model::M3D, model::A3D> _model;
private slots:
	void onTreeItemClicked(QTreeWidgetItem *item, int column);
};

#endif // MODELVIEWER_H