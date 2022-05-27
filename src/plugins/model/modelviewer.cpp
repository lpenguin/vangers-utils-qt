#include "modelviewer.h"
#include "propertytree.h"
#include "scenecontroller.h"
#include "modelviewerplugin.h"

#include <core/vector/vectorjsonext.h>


#include <Qt3DExtras/Qt3DWindow>
#include <QFileInfo>
#include <QDir>
#include <QPushButton>
#include <QMenu>
#include <QClipboard>
#include <plugins/model/objimport/objimportsettingsaccess.h>
#include <plugins/model/objimport/objexporter.h>
#include <plugins/model/objimport/objimporter.h>
#include <plugins/model/m3d/modelaccess.h>
#include <plugins/model/obj/objreader.h>
#include <plugins/model/obj/objwriter.h>

using namespace vangers::model;
using namespace vangers::model::view;
using namespace vangers::model::obj;
using namespace vangers::model::objimport;

ModelViewer::ModelViewer(ResourceViewerPlugin *plugin, QWidget *parent)
	: ResourceViewer(plugin, parent)
	, _currentFile()
	, _ui(new Ui::ModelViewer())
	, _a3dModelIndex(0)
{
	_ui->setupUi(this);
	connect(_ui->propertiesTree, &QTreeWidget::itemClicked,
			this, &ModelViewer::onTreeItemClicked);

	_ui->propertiesTree->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(
		_ui->propertiesTree,
		&QTreeWidget::customContextMenuRequested,
		this,
		&ModelViewer::onTreeContextMenuRequested
	);

	Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();

	_widget3d = QWidget::createWindowContainer(view);
	_ui->splitter->insertWidget(0, _widget3d);

	_objImportWidget = new ObjImportSettingsWidget(_ui->tabObjImporter);
	// TODO: memory leak
	_objImportController = new ObjImportController(_objImportWidget);
	_sceneController = new SceneController(view, this);
	_objectsController = new M3DObjectsController(_ui->treeObjects, _sceneController, this);

	QObject::connect(_ui->resetViewButton,
					 &QAbstractButton::clicked,
					 _sceneController,
					 &SceneController::resetView);

	QObject::connect(_ui->prevModelButton,
					 &QAbstractButton::clicked,
					 this,
					 &ModelViewer::onPrevModelClicked);

	QObject::connect(_ui->nextModelButton,
					 &QAbstractButton::clicked,
					 this,
					 &ModelViewer::onNextModelClicked);

	QObject::connect(_objImportWidget,
					 &ObjImportSettingsWidget::importSettingsChanged,
					 this,
					 &ModelViewer::onImportSettingsChanged);
}



bool ModelViewer::importResource(const QString &filePath, const ResourceType &type)
{
	_currentFile = filePath;
	_ui->propertiesTree->clear();
	_ui->tabWidget->setTabVisible(2, false);

	if(type == ModelViewerPlugin::M3D){
		return importM3D(filePath);
	} else if(type == ModelViewerPlugin::Obj)  {
		_ui->tabWidget->setTabVisible(2, true);
		return importObj(filePath);
	}

	return false;
}



void ModelViewer::exportResource(const QString &filePath, const ResourceType &type)
{
	if(!_model) return;

	if(type == ModelViewerPlugin::Obj) {
		exportObj(filePath);
	} else if(type == ModelViewerPlugin::M3D){
		exportM3D(filePath);
	}
}

void ModelViewer::showA3dModel(A3D& a3d)
{
	if(_a3dModelIndex < 0 || _a3dModelIndex >= a3d.models.size()) return;

	// TODO:
	//	_sceneController->setC3D(a3d.models[_a3dModelIndex]);
}



void ModelViewer::setupModel()
{
	const M3D& m3d = *_model;

	_ui->propertiesTree->clear();
	auto* root = addProperty(nullptr, "root", m3d);
	_sceneController->setM3D(m3d);
	_objectsController->setM3D(m3d);

	_ui->propertiesTree->addTopLevelItem(root);
	_ui->propertiesTree->setColumnWidth(0, 200);

	root->setExpanded(true);
	root->child(0)->setExpanded(true);
}

bool ModelViewer::importM3D(const QString& filePath)
{
	_model = QSharedPointer<M3D>::create();

	M3DAccess access;

	if(!access.readFromFile(*_model, filePath)){
		return false;
	}

	setupModel();
	return true;
}

QString getImportSettingsFilename(const QString& filePath){
	QFileInfo fileInfo(filePath);
	QString settingsName = fileInfo.baseName() + ".import.ini";
	return fileInfo.absoluteDir().filePath(settingsName);
}

QString getMaterialsFilename(const QString& filePath){
	QFileInfo fileInfo(filePath);
	QString settingsName = fileInfo.baseName() + ".mtl";
	return fileInfo.absoluteDir().filePath(settingsName);
}

bool ModelViewer::importObj(const QString& filePath)
{
	_model = QSharedPointer<M3D>::create();

	std::unique_ptr<ObjectCollection> objectCollection = std::make_unique<ObjectCollection>();


	QFile f(filePath);
	if(!f.open(QFile::ReadOnly | QFile::Text)){
		return false;
	}

	if(!ObjReader().read(*objectCollection, f)) return false;


	QString settingsFileName = getImportSettingsFilename(filePath);

	QFileInfo fileInfo(settingsFileName);


	std::unique_ptr<ObjImportSettings> importSettings = std::make_unique<ObjImportSettings>();
	ObjImporter::makeDefaultImportSettings(*objectCollection, *importSettings);

	if(fileInfo.exists()){
		ObjImportSettingsAccess settingsAccess;
		ObjImportSettings readImportSettings{};
		if(!settingsAccess.readFromFile(readImportSettings, settingsFileName)){
			qDebug() << "Could not read import settings" << settingsFileName;
			importSettings = {};
		}
		ObjImportSettings::merge(readImportSettings, *importSettings);
	}

	ObjImportSettingsAccess().writeToFile(*importSettings, getImportSettingsFilename(filePath));

	_objImportController->setObjectCollection(
				std::move(objectCollection),
				std::move(importSettings));

	bool ok = _objImportController->import(*_model);
	if(!ok) return false;

	setupModel();
	return true;
}

void ModelViewer::exportM3D(const QString& filePath)
{
	M3DAccess().writeToFile(*_model, filePath);
}

void ModelViewer::exportObj(const QString& filePath)
{
	ObjectCollection result;
	ObjImportSettings settings{};
	ObjExporter().export_(*_model, result, settings);
	ObjImportSettingsAccess().writeToFile(settings, getImportSettingsFilename(filePath));

	QString mtlFilePath = getMaterialsFilename(filePath);
	QString mtlName = QFileInfo(mtlFilePath).fileName();
	result.useMaterialLibraries.append(mtlName);

	ObjWriter().write(result, filePath);
	ObjWriter().writeMaterials(result.materials, mtlFilePath);

}

QVector3D fromVectorI8(const Vector3I8& v){
	return QVector3D(v.x, v.y, v.z);
}

void ModelViewer::onTreeItemClicked(QTreeWidgetItem* item, int column)
{
	QStringList path;
	do{
		path.prepend(item->text(0));
		item = item->parent();
	}while(item->parent() != nullptr);
}

void ModelViewer::onTreeContextMenuRequested(const QPoint& pos)
{
	qDebug() << _ui->propertiesTree->selectedItems().size();
	QMenu* menu = new QMenu(this);
	QAction* copyContentsAction = new QAction("Copy selection contents", this);
	connect(copyContentsAction,
			&QAction::triggered,
			[this](bool){
		QStringList contentsList;
		for(const QTreeWidgetItem* item: _ui->propertiesTree->selectedItems()){
			QStringList rowContentsList;
			for(int i = 0; i < item->columnCount(); i++){
				rowContentsList.append(item->text(i));
			}
			contentsList.append(rowContentsList.join('\t'));
		}
		 QClipboard *clipboard = QGuiApplication::clipboard();
		 clipboard->setText(contentsList.join("\n"));
	});

	menu->addAction(copyContentsAction);
	menu->exec(_ui->propertiesTree->mapToGlobal(pos) );
}

void ModelViewer::onPrevModelClicked()
{
//	if(std::holds_alternative<model::A3D>(_model)){
//		auto& a3d = std::get<model::A3D>(_model);
//		_a3dModelIndex = _a3dModelIndex - 1;
//		if(_a3dModelIndex < 0){
//			_a3dModelIndex = a3d.models.size() - 1;
//		}
//		showA3dModel(a3d);
//	}
}

void ModelViewer::onNextModelClicked()
{
//	if(std::holds_alternative<model::A3D>(_model)){
//		auto& a3d = std::get<model::A3D>(_model);
//		_a3dModelIndex = (_a3dModelIndex + 1) % a3d.models.size();
//		showA3dModel(a3d);
//	}
}

void ModelViewer::onImportSettingsChanged()
{
	bool ok = _objImportController->import(*_model);
	if(!ok) return;

	ObjImportSettingsAccess().writeToFile(*_objImportController->importSettings(), getImportSettingsFilename(_currentFile));

	setupModel();
}



