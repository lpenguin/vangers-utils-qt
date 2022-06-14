#ifndef PROPERTYTREE_H
#define PROPERTYTREE_H

#include <QTreeWidgetItem>
#include <plugins/model/m3d/m3d.h>

const int NAME_COLUMN = 0;
const int TYPE_COLUMN = 2;
const int VALUE_COLUMN = 1;

template<typename T>
void setupProperty(QTreeWidgetItem* item, const T& property);

template<typename T>
QTreeWidgetItem* addProperty(QTreeWidgetItem* parent, const QString& name, const T& property){
	QTreeWidgetItem* item = new QTreeWidgetItem(parent);
	item->setText(NAME_COLUMN, name);
	setupProperty(item, property);
	if(parent != nullptr){
		parent->addChild(item);
	}
	return item;
}

template<>
void setupProperty(QTreeWidgetItem* item, const int32_t& property);

template<>
void setupProperty(QTreeWidgetItem* item, const uint32_t& property);

template<>
void setupProperty(QTreeWidgetItem* item, const uint8_t& property);

template<>
void setupProperty(QTreeWidgetItem* item, const int8_t& property);

template<>
void setupProperty(QTreeWidgetItem* item, const float& property);

template<>
void setupProperty(QTreeWidgetItem* item, const double& property);

template<>
void setupProperty(QTreeWidgetItem* item, const vangers::core::vector::Vector3I32& vector);

template<>
void setupProperty(QTreeWidgetItem* item, const vangers::core::vector::Vector3I8& vector);

template<>
void setupProperty(QTreeWidgetItem* item, const vangers::core::vector::Vector3F32& vector);

template<>
void setupProperty(QTreeWidgetItem* item, const vangers::core::vector::Vector3F64& vector);

template<>
void setupProperty(QTreeWidgetItem* item, const vangers::core::vector::Matrix3x3F64& property);

template<>
void setupProperty(QTreeWidgetItem* item, const vangers::model::m3d::Vertex& property);

template<>
void setupProperty(QTreeWidgetItem* item, const vangers::model::m3d::Normal& property);

template<>
void setupProperty(QTreeWidgetItem* item, const vangers::model::m3d::PolygonNormal& property);

template<typename I>
void setupProperty(QTreeWidgetItem* item, const std::vector<I>& property){
	item->setText(TYPE_COLUMN, QString("vector[%1]").arg(property.size()));

	for(size_t i = 0; i < property.size(); i++){
		addProperty(item, QString("[%1]").arg(i), property[i]);
	}
}

template<>
void setupProperty(QTreeWidgetItem* item, const vangers::model::m3d::PolygonIndex& property);

template<>
void setupProperty(QTreeWidgetItem* item, const vangers::model::m3d::Polygon& property);


template<>
void setupProperty(QTreeWidgetItem* item, const vangers::model::m3d::C3D& c3d);

template<>
void setupProperty(QTreeWidgetItem* item, const vangers::model::m3d::M3D& property);

template<>
void setupProperty(QTreeWidgetItem* item, const vangers::model::m3d::A3D& property);


#endif // PROPERTYTREE_H
