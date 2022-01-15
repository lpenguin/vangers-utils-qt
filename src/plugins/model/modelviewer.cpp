#include "modelviewer.h"
#include "modelaccess.h"
#include "drawdata.h"
#include "propertytree.h"

#include <QQmlContext>
#include <QQuickItem>

ModelViewer::ModelViewer(ResourceViewerPlugin *plugin, QWidget *parent)
	: ResourceViewer(plugin, parent)
	, _currentFile()
	, _ui(new Ui::ModelViewer())
{
	_ui->setupUi(this);
}



bool ModelViewer::importResource(const QString &filePath, const ResourceType &)
{
	_currentFile = filePath;
	ModelAccess access;
	model::M3D m3d;

	if(!access.readFromFile(m3d, filePath)){
		return false;
	}

	_ui->propertiesTree->clear();

	QTreeWidgetItem* root = addProperty(nullptr, "root", m3d);

	_ui->propertiesTree->addTopLevelItem(root);
//	_ui->propertiesTree->expandItem(root);
//	_ui->propertiesTree->expandItem(root);
	_ui->propertiesTree->setColumnWidth(0, 200);

	root->setExpanded(true);
	root->child(0)->setExpanded(true);

//	QQmlContext* rootContext = _ui->quickWidget->rootContext();
	QQuickItem* rootObject = _ui->quickWidget->rootObject();

	DrawData* vertexData = rootObject->property("vertexData").value<DrawData*>();
//	vertexData->setData({
//						  {{-2.0, -3.0, -2.0}, {1, 1, 1}},
//						  {{2.0, -2.0, -2.0}, {1, 1, 1}},
//						  {{2.0,  3.0, -2.0}, {1, 1, 1}},
//						  {{-2.0,  2.0, -2.0}, {1, 1, 1}},
//					  });

	QVector<DrawVBOData> vertices;
	QVector<uint32_t> indices;

	adapter.adapt(m3d, vertices, indices);

	vertexData->setData(vertices);
	DrawData* indexData = rootObject->property("indexData").value<DrawData*>();
//	indexData->setData({0, 1, 2, 0, 2, 3});
	indexData->setData(indices);
	qDebug() << indexData->count();
//	0, 1, 2,
//	0, 2, 3,
	return true;
//	Qt3DRender::QBuffer* vertexBuffer = rootObject->findChild<Qt3DRender::QBuffer*>("vertexBuffer");
//	qDebug() << (vertexBuffer == nullptr);

//	qDebug() << vertexBuffer;

	if(vertexData == nullptr){
		qWarning() << "Cannot find draw data object";
		return false;
	}

//	if(vertexBuffer == nullptr){
//		qWarning() << "Cannot find vertexBuffer object";
//		return false;
//	}


//	qDebug() << "vertexBuffer: " << vertexBuffer->data();
	qDebug() << "drawData.buffer: " << vertexData->buffer()->data();


//	DrawData* drawData = new DrawData();

//	QObject* co = rootContext->contextObject();
//	co->dumpObjectTree();
//	co->dumpObjectInfo();
	return true;
}



void ModelViewer::exportResource(const QString &filePath, const ResourceType &)
{

}


