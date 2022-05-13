#include "modelviewer.h"
#include "modelaccess.h"
#include "propertytree.h"
#include "scenecontroller.h"
#include "modelviewerplugin.h"
#include "modelobjaccess.h"
#include "extensions/jsonext.h"

#include <Qt3DExtras/Qt3DWindow>
#include <QPushButton>


ModelViewer::ModelViewer(ResourceViewerPlugin *plugin, QWidget *parent)
	: ResourceViewer(plugin, parent)
	, _currentFile()
	, _ui(new Ui::ModelViewer())
	, _a3dModelIndex(0)
{
	_ui->setupUi(this);
	connect(_ui->propertiesTree, &QTreeWidget::itemClicked,
			this, &ModelViewer::onTreeItemClicked);

	Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();

	_widget3d = QWidget::createWindowContainer(view);
	_ui->splitter->insertWidget(0, _widget3d);
	_sceneController = new SceneController(view, this);

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
}



bool ModelViewer::importResource(const QString &filePath, const ResourceType &type)
{
	_currentFile = filePath;
	_ui->propertiesTree->clear();

	QTreeWidgetItem* root = nullptr;

	if(type == ModelViewerPlugin::M3D){
		ModelM3DAccess access;
		_model = model::M3D();

		model::M3D& m3d = std::get<model::M3D>(_model);

		if(!access.readFromFile(m3d, filePath)){
			return false;
		}

		root = addProperty(nullptr, "root", m3d);
		_sceneController->setM3D(QSharedPointer<model::M3D>::create(m3d));
	} else if(type == ModelViewerPlugin::A3D){
		ModelA3DAccess access;
		_model = model::A3D();
		_a3dModelIndex = 0;

		model::A3D& a3d = std::get<model::A3D>(_model);

		if(!access.readFromFile(a3d, filePath)){
			return false;
		}

		root = addProperty(nullptr, "root", a3d);
		_sceneController->setC3D(a3d.models[0]);
	} else if(type == ModelViewerPlugin::Json) {
		QFile file(filePath);
		file.open(QFile::ReadOnly);
		std::string s = file.readAll().toStdString();
		file.close();

		auto json = nlohmann::json::parse(s);
		if(json.contains("models")){
			_model = model::A3D();
			auto& a3d = std::get<model::A3D>(_model);
			from_json(json, a3d);
			_sceneController->setC3D(a3d.models[0]);
			root = addProperty(nullptr, "root", a3d);
		} else {
			_model = model::M3D();

			auto& m3d = std::get<model::M3D>(_model);
			from_json(json, m3d);
			root = addProperty(nullptr, "root", m3d);
			_sceneController->setM3D(QSharedPointer<model::M3D>::create(m3d));
		}

	} else {
		return false;
	}

	_ui->propertiesTree->addTopLevelItem(root);
	_ui->propertiesTree->setColumnWidth(0, 200);

	root->setExpanded(true);
	root->child(0)->setExpanded(true);


	return true;
}



void ModelViewer::exportResource(const QString &filePath, const ResourceType &type)
{
	if(type.name == ModelViewerPlugin::Json.name){
		if(std::holds_alternative<model::M3D>(_model)){
			const model::M3D& m3d = std::get<model::M3D>(_model);
			auto root = nlohmann::json::object();
			to_json(root, m3d);
			std::string serialized = root.dump(2);
			QFile f(filePath);
			f.open(QFile::WriteOnly);
			f.write(serialized.c_str());
			f.close();
		} else if(std::holds_alternative<model::A3D>(_model)){
			const model::A3D& a3d = std::get<model::A3D>(_model);
			auto root = nlohmann::json::object();
			to_json(root, a3d);
			std::string serialized = root.dump(2);
			QFile f(filePath);
			f.open(QFile::WriteOnly);
			f.write(serialized.c_str());
			f.close();
		}
	} else if(type == ModelViewerPlugin::Obj) {
		if(std::holds_alternative<model::M3D>(_model)){
			const model::M3D& m3d = std::get<model::M3D>(_model);
			vangers::ModelObjAccess().write(m3d, filePath);
		}
	}
}

void ModelViewer::showA3dModel(model::A3D& a3d)
{
	if(_a3dModelIndex < 0 || _a3dModelIndex >= a3d.models.size()) return;

	_sceneController->setC3D(a3d.models[_a3dModelIndex]);
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

	if(std::holds_alternative<model::A3D>(_model)){
		qDebug() << path;
		if(path.size() >= 2&& path[0] == "models"){
			QString modelIndStr = path[1];
			_a3dModelIndex = modelIndStr.mid(1, modelIndStr.size() - 2).toInt();
			showA3dModel(std::get<model::A3D>(_model));
		}

	}
	// TODO: body parts selection


}

void ModelViewer::onPrevModelClicked()
{
	if(std::holds_alternative<model::A3D>(_model)){
		auto& a3d = std::get<model::A3D>(_model);
		_a3dModelIndex = _a3dModelIndex - 1;
		if(_a3dModelIndex < 0){
			_a3dModelIndex = a3d.models.size() - 1;
		}
		showA3dModel(a3d);
	}
}

void ModelViewer::onNextModelClicked()
{
	if(std::holds_alternative<model::A3D>(_model)){
		auto& a3d = std::get<model::A3D>(_model);
		_a3dModelIndex = (_a3dModelIndex + 1) % a3d.models.size();
		showA3dModel(a3d);
	}
}


