#include "objimportsettingswidget.h"
#include "ui_objimportsettingswidget.h"

#include <plugins/model/m3d/colortable.h>

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QItemDelegate>
#include <QLineEdit>
#include <QDebug>

using namespace vangers::model::objimport;
using namespace vangers::model::obj;
using namespace vangers::model::m3d;



class DoubleDelegate : public QItemDelegate
{
public:
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
					  const QModelIndex& index) const
	{
		if(index.column() != 1) {
			return nullptr;
		}

		QLineEdit *lineEdit = new QLineEdit(parent);
		QDoubleValidator *validator = new QDoubleValidator(lineEdit);
		validator->setNotation(QDoubleValidator::StandardNotation);
		validator->setLocale(QLocale::c());
		lineEdit->setValidator(validator);

		return lineEdit;
	}
};

ObjImportSettingsWidget::ObjImportSettingsWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::ObjImportSettingsWidget)
{
	ui->setupUi((QWidget*) this);
	ui->treeParameters->setItemDelegate(new DoubleDelegate());

	QObject::connect(
		ui->buttonReimport,
		&QPushButton::clicked,
		this,
		&ObjImportSettingsWidget::handleReimportClicked
	);
}

ObjImportSettingsWidget::~ObjImportSettingsWidget()
{
	delete ui;
}

void ObjImportSettingsWidget::setImportSettings(const ObjImportSettings& importSettings){

	setupTables(importSettings);
}

void ObjImportSettingsWidget::makeImportSettings(ObjImportSettings& importSettings)
{
	makeObjects(importSettings);
	makeMaterials(importSettings);
	makeParameters(importSettings);
}


void ObjImportSettingsWidget::handleReimportClicked()
{
	emit importSettingsChanged();
}

void ObjImportSettingsWidget::setupTables(const ObjImportSettings& importSettings)
{
	setupObjetcs(importSettings);
	setupMaterials(importSettings);
	setupParameters(importSettings);

//	ui->tableParameters->setRowCount(0);
//	ui->tableParameters->setRowCount(2);

//	QDoubleSpinBox* scaleSpinBox = new QDoubleSpinBox(nullptr);
//	scaleSpinBox->setValue(_importSettings.customScale.valueOrDefault(1.0));

//	QTableWidgetItem* nameItem = new QTableWidgetItem("scale");
//	ui->tableParameters->setItem(0, 0, nameItem);
//	ui->tableParameters->setCellWidget(0, 1, scaleSpinBox);


//	QDoubleSpinBox* volumeSpinBox = new QDoubleSpinBox(nullptr);
//	scaleSpinBox->setValue(_importSettings.customVolume.valueOrDefault(1.0));

//	QTableWidgetItem* volumeItem = new QTableWidgetItem("volume");
//	ui->tableParameters->setItem(1, 0, volumeItem);
	//	ui->tableParameters->setCellWidget(1, 1, volumeSpinBox);
}

void ObjImportSettingsWidget::setupObjetcs(const ObjImportSettings& importSettings)
{
	ui->tableObjects->setRowCount(0);

	for(const QString& name: importSettings.objectMapping.keys()){
		int row = ui->tableObjects->rowCount();
		ui->tableObjects->setRowCount(row + 1);

		QTableWidgetItem* nameItem = new QTableWidgetItem(name);
		ui->tableObjects->setItem(row, 0, nameItem);
		QComboBox* typeCombo = new QComboBox(nullptr);
		for(int i = 0; i < (int32_t)ObjectType::MAX_VALUE; i++){
			typeCombo->addItem(ObjectNames[i]);
		}
		ObjectType type = importSettings.objectMapping[name];
		typeCombo->setCurrentIndex((int32_t)type);

		ui->tableObjects->setCellWidget(row, 1, typeCombo);
	}
}

void ObjImportSettingsWidget::setupMaterials(const ObjImportSettings& importSettings)
{
	ui->tableMaterials->setRowCount(0);

	for(const QString& material: importSettings.materialsMapping.keys()){
		int row = ui->tableMaterials->rowCount();
		ui->tableMaterials->setRowCount(row + 1);

		QTableWidgetItem* nameItem = new QTableWidgetItem(material);
		ui->tableMaterials->setItem(row, 0, nameItem);

		QComboBox* typeCombo = new QComboBox(nullptr);
		for(int i = 0; i < (int32_t)ColorId::MAX_VALUE; i++){
			typeCombo->addItem(ColorNames[i]);
		}
		ColorId colorId = importSettings.materialsMapping[material];
		typeCombo->setCurrentIndex((int32_t)colorId);

		ui->tableMaterials->setCellWidget(row, 1, typeCombo);
	}
}

void setupItem(const float& param, QTreeWidgetItem* item){
//	QTreeWidgetItem* child = nullptr;
//	child = new QTreeWidgetItem(item);
//	child->setText(0, "Value");
//	child->setText(1, QString::number(param));
//	child->setFlags(child->flags() | Qt::ItemIsEditable);
	item->setText(1, QString::number(param));
	item->setFlags(item->flags() | Qt::ItemIsEditable);

}

template<typename T>
void setupItem(const Vector3<T> param, QTreeWidgetItem* item){
	QTreeWidgetItem* child = nullptr;

	child = new QTreeWidgetItem(item);
	child->setText(0, "x");
	child->setText(1, QString::number(param.x, 'f'));
	child->setFlags(child->flags() | Qt::ItemIsEditable);

	child = new QTreeWidgetItem(item);
	child->setText(0, "y");
	child->setText(1, QString::number(param.y, 'f'));
	child->setFlags(child->flags() | Qt::ItemIsEditable);

	child = new QTreeWidgetItem(item);
	child->setText(0, "z");
	child->setText(1, QString::number(param.z, 'f'));
	child->setFlags(child->flags() | Qt::ItemIsEditable);
}

template<typename T>
void setupItem(const Matrix3x3<T> param, QTreeWidgetItem* item){
	for(int i =0; i < 9; i++){
		QTreeWidgetItem* child = nullptr;

		child = new QTreeWidgetItem(item);
		child->setText(0, QString("[%1]").arg(i));
		child->setText(1, QString::number(param.values[i], 'f'));
		child->setFlags(child->flags() | Qt::ItemIsEditable);
	}
}

template<typename T>
void addParameter(const Optional<T>& param,
				  const QString& name,
				  QTreeWidget* treeWidget){
	QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setText(0, name);

	QString text;
	if(param.hasValue()){
		item->setCheckState(0, Qt::Checked);
	} else {
		item->setCheckState(0, Qt::Unchecked);
	}

	setupItem(param.valueConst(), item);
	treeWidget->addTopLevelItem(item);
}

template<typename T>
void itemToParameter(QTreeWidgetItem* item, Optional<T>& param){
	if(item->checkState(0) == Qt::Unchecked){
		param = Optional<T>();
	} else {
		param = Optional<T>(T{});
		itemToParameter(item, param.value());
	}
}

void itemToParameter(QTreeWidgetItem* item, float& param){
	bool isOk = false;
	param = item->text(1).toFloat(&isOk);
	if(!isOk) param = 0;
}

void itemToParameter(QTreeWidgetItem* item, double& param){
	bool isOk = false;
	param = item->text(1).toDouble(&isOk);
	if(!isOk) param = 0;
}

void itemToParameter(QTreeWidgetItem* item, Vector3F64& param){
	double* params[3] = {
		&param.x,
		&param.y,
		&param.z
	};
	if(item->childCount() != 3) return;

	for (int i = 0; i < 3; i++ ) {
		QTreeWidgetItem* child = item->child(i);
		itemToParameter(child, *params[i]);
	}
}

void itemToParameter(QTreeWidgetItem* item, Matrix3x3F64& param){
	if(item->childCount() != 9) return;

	for (int i = 0; i < 9; i++ ) {
		QTreeWidgetItem* child = item->child(i);
		itemToParameter(child, param.values[i]);
	}
}

void ObjImportSettingsWidget::setupParameters(const ObjImportSettings& importSettings)
{
	while(ui->treeParameters->topLevelItemCount() > 0){
		ui->treeParameters->takeTopLevelItem(0);
	}
	addParameter(importSettings.customScale, "Custom Scale", ui->treeParameters);
	addParameter(importSettings.customVolume, "Custom Volume", ui->treeParameters);
	addParameter(importSettings.customRcm, "Custom RCM", ui->treeParameters);
	addParameter(importSettings.customJ, "Custom J", ui->treeParameters);

	ui->treeParameters->setColumnWidth(0, 220);

}

void ObjImportSettingsWidget::makeObjects(ObjImportSettings& importSettings)
{
	for(int row = 0; row < ui->tableObjects->rowCount(); row++){
		QTableWidgetItem* nameItem = ui->tableObjects->item(row, 0);
		QComboBox* typeCombo = (QComboBox* )ui->tableObjects->cellWidget(row, 1);
		int typeIndex = typeCombo->currentIndex();
		if(typeIndex < 0 || typeIndex >= (int32_t)ObjectType::MAX_VALUE){
			continue;
		}

		ObjectType type = (ObjectType)typeIndex;
		QString objectName = nameItem->text();
		importSettings.objectMapping[objectName] = type;
	}
}

void ObjImportSettingsWidget::makeMaterials(ObjImportSettings& importSettings)
{
	for(int row = 0; row < ui->tableMaterials->rowCount(); row++){
		QTableWidgetItem* nameItem = ui->tableMaterials->item(row, 0);
		QComboBox* colorCombo = (QComboBox* )ui->tableMaterials->cellWidget(row, 1);
		int colorIndex = colorCombo->currentIndex();
		if(colorIndex < 0 || colorIndex >= (int32_t)ColorId::MAX_VALUE){
			continue;
		}

		ColorId colorId = (ColorId)colorIndex;
		QString materialName = nameItem->text();
		importSettings.materialsMapping[materialName] = colorId;
	}
}

void ObjImportSettingsWidget::makeParameters(ObjImportSettings& importSettings)
{
	int len = ui->treeParameters->topLevelItemCount();
	for(int i = 0; i < len; i++){
		QTreeWidgetItem* item = ui->treeParameters->topLevelItem(i);
		QString text = item->text(0);
		if(text == "Custom Scale"){
			itemToParameter(item, importSettings.customScale);
		} else if(text == "Custom Volume"){
			itemToParameter(item, importSettings.customVolume);
		} else if(text == "Custom RCM"){
			itemToParameter(item, importSettings.customRcm);
		} else if(text == "Custom J"){
			itemToParameter(item, importSettings.customJ);
		}
	}
}
