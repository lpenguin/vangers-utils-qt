#include "modelviewer.h"
#include "modelaccess.h"
#include "propertytree.h"
#include "scenecontroller.h"

#include <Qt3DExtras/Qt3DWindow>


ModelViewer::ModelViewer(ResourceViewerPlugin *plugin, QWidget *parent)
	: ResourceViewer(plugin, parent)
	, _currentFile()
	, _ui(new Ui::ModelViewer())
{
	_ui->setupUi(this);
	connect(_ui->propertiesTree, &QTreeWidget::itemClicked,
			this, &ModelViewer::onTreeItemClicked);

	Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();

	_widget3d = QWidget::createWindowContainer(view);
	_ui->splitter->insertWidget(0, _widget3d);
	_sceneController = new SceneController(view, this);

}



bool ModelViewer::importResource(const QString &filePath, const ResourceType &)
{
	_currentFile = filePath;
	ModelAccess access;

	if(!access.readFromFile(_m3d, filePath)){
		return false;
	}

	_ui->propertiesTree->clear();

	QTreeWidgetItem* root = addProperty(nullptr, "root", _m3d);

	_ui->propertiesTree->addTopLevelItem(root);
//	_ui->propertiesTree->expandItem(root);
//	_ui->propertiesTree->expandItem(root);
	_ui->propertiesTree->setColumnWidth(0, 200);

	root->setExpanded(true);
	root->child(0)->setExpanded(true);
	_sceneController->setM3D(QSharedPointer<model::M3D>::create(_m3d));


	return true;
}



void ModelViewer::exportResource(const QString &filePath, const ResourceType &)
{

}

QVector3D fromVectorI8(const model::Vector3I8& v){
	return QVector3D(v.x, v.y, v.z);
}

void ModelViewer::onTreeItemClicked(QTreeWidgetItem* item, int column)
{
	QStringList path;
	do{
		path.prepend(item->text(0));
		item = item->parent();
	}while(item->parent() != nullptr);

	// TODO: body parts selection

//	qDebug() << path;

//	if(path.size() >= 3&& path[0] == "body" && path[1] == "polygons"){
//		QString polyIndStr = path[2];
//		int polygonIndex = polyIndStr.mid(1, polyIndStr.size() - 2).toInt();
//		qDebug() << polygonIndex;
//		model::Polygon& polygon = _m3d.body.polygons[polygonIndex];
//		if(polygon.indices.size() != 3){
//			qDebug() << "Wrong polygon size: " << polygon.indices.size();
//			return;
//		}

//		QVector3D positions[3] = {
//			fromVectorI8(_m3d.body.vectices[polygon.indices[0].vertInd].pos),
//			fromVectorI8(_m3d.body.vectices[polygon.indices[1].vertInd].pos),
//			fromVectorI8(_m3d.body.vectices[polygon.indices[2].vertInd].pos),
//		};
//		qDebug() << positions[0] << positions[1] << positions[2];
//		_sceneController->setPoly(positions);
//	}
}


