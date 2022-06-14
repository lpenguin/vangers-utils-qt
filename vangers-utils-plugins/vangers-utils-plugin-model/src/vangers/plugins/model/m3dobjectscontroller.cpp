#include "m3dobjectscontroller.h"

#include <functional>

using namespace vangers::plugins::model;
using namespace m3d;

class CheckedClickHandler {
public:
	CheckedClickHandler()
		: _clickFunc()
	{

	}
	~CheckedClickHandler() = default;
	CheckedClickHandler(const CheckedClickHandler& other) = default;

	CheckedClickHandler(std::function<void (QTreeWidgetItem*, bool checked)> clickFunc)
		: _clickFunc(clickFunc)
	{

	}

	void operator()(QTreeWidgetItem*item, bool checked)
	{
		if(!_clickFunc) return;

		_clickFunc(item, checked);
	}

private:
	std::function<void (QTreeWidgetItem*, bool checked)> _clickFunc;
};
Q_DECLARE_METATYPE(CheckedClickHandler);


M3DObjectsController::M3DObjectsController(QTreeWidget* treeWidget, SceneController* sceneController, QObject* parent)
	: QObject(parent)
	, _treeWidget(treeWidget)
	, _sceneController(sceneController)
{
	QObject::connect(
				_treeWidget,
				&QTreeWidget::itemChanged,
				this,
				&M3DObjectsController::onItemClicked);

}

void M3DObjectsController::setM3D(const M3D& m3d)
{
	initializeTree(m3d);
}

void M3DObjectsController::onItemClicked(QTreeWidgetItem* item, int column)
{
	bool checked = item->checkState(column) == Qt::Checked;
	QVariant itemData = item->data(column, Qt::UserRole);
	auto handler = itemData.value<CheckedClickHandler>();
	handler(item, checked);
}

void setClickHandler(QTreeWidgetItem* item, CheckedClickHandler handler){
	QVariant data;
	data.setValue<CheckedClickHandler>(handler);
	item->setData(0, Qt::UserRole, data);
}

QTreeWidgetItem* createTreeItem(
		QTreeWidgetItem* parent,
		QString name,
		bool checked,
		CheckedClickHandler handler){
	QTreeWidgetItem* item = new QTreeWidgetItem(parent);
	item->setText(0, name);
	item->setCheckState(0, checked ? Qt::Checked : Qt::Unchecked);

	setClickHandler(item, handler);
	handler(item, checked);
	return item;
}

void setChildrenChecked(QTreeWidgetItem* parent, Qt::CheckState checked){
	for(int i = 0; i < parent->childCount(); i++){
		parent->child(i)->setCheckState(0, checked);
	}
}

void M3DObjectsController::initializeTree(const M3D& m3d)
{
	_treeWidget->clear();
	_treeWidget->setColumnCount(1);

	QList<QTreeWidgetItem*> topLevelItems;

	CheckedClickHandler setChildrenCheckedHandler([](auto* item, bool checked){
		setChildrenChecked(item, item->checkState(0));
	});

	QTreeWidgetItem* bodyItem = createTreeItem(nullptr, "body", true, setChildrenCheckedHandler);
	topLevelItems.append(bodyItem);


	createTreeItem(bodyItem, "model", true, {[this](auto*, bool checked){
		_sceneController->setBodyVisible(checked);
	}});

	createTreeItem(bodyItem, "bound", false, {[this](auto*, bool checked){
		_sceneController->setBoundVisible(checked);
	}});

	QTreeWidgetItem* debrisListItem = createTreeItem(nullptr, "debris", true, setChildrenCheckedHandler);

	topLevelItems.append(debrisListItem);
	for(int i = 0; i < m3d.debris.size(); i++){
		QString name("debris[%1]");
		QTreeWidgetItem* debrisItem = createTreeItem(debrisListItem, name.arg(i), false, setChildrenCheckedHandler);

		createTreeItem(debrisItem, "model", false, {[this, i](auto*, bool checked){
			_sceneController->setDebrisVisible(i, checked);
		}});

		createTreeItem(debrisItem, "bound", false, {[this, i](auto*, bool checked){
			_sceneController->setDebrisBoundVisible(i, checked);
		}});

//		createTreeItem(debreeItem, "bound", false, {});
	}

	QTreeWidgetItem* wheelsListItem = createTreeItem(nullptr, "wheels", true, setChildrenCheckedHandler);;
	topLevelItems.append(wheelsListItem);
	for(int i = 0; i < m3d.wheels.size(); i++){
		if(m3d.wheels[i].model.size() == 0) continue;

		QString name("wheel[%1]");
		QTreeWidgetItem* wheelItem = createTreeItem(wheelsListItem, name.arg(i), true, {[this, i](auto*, bool checked){
			_sceneController->setWheelVisible(i, checked);
		}});
	}

	QTreeWidgetItem* slotListItem = createTreeItem(nullptr, "slots", true, setChildrenCheckedHandler);;
	topLevelItems.append(slotListItem);
	for(int i = 0; i < _sceneController->slotCount(); i++){
		QString name("slot[%1]");
		createTreeItem(slotListItem, name.arg(i), true, {[this, i](auto*, bool checked){
			_sceneController->setSlotVisible(i, checked);
		}});
	}

	_treeWidget->addTopLevelItems(topLevelItems);
	_treeWidget->expandAll();
}
