#include "propertytree.h"


template<>
void setupProperty(QTreeWidgetItem* item, const int32_t& property){
	item->setText(TYPE_COLUMN, "int32");
	item->setText(VALUE_COLUMN, QString("%1").arg(property));
}

template<>
void setupProperty(QTreeWidgetItem* item, const uint32_t& property){
	item->setText(TYPE_COLUMN, "uint32_t");
	item->setText(VALUE_COLUMN, QString("%1").arg(property));
}

template<>
void setupProperty(QTreeWidgetItem* item, const uint8_t& property){
	item->setText(TYPE_COLUMN, "uint8");
	item->setText(VALUE_COLUMN, QString("%1").arg(property));
}

template<>
void setupProperty(QTreeWidgetItem* item, const int8_t& property){
	item->setText(TYPE_COLUMN, "int8");
	item->setText(VALUE_COLUMN, QString("%1").arg(property));
}

template<>
void setupProperty(QTreeWidgetItem* item, const float& property){
	item->setText(TYPE_COLUMN, "float32");
	item->setText(VALUE_COLUMN, QString("%1").arg(property));
}

template<>
void setupProperty(QTreeWidgetItem* item, const double& property){
	item->setText(TYPE_COLUMN, "float64");
	item->setText(VALUE_COLUMN, QString("%1").arg(property));
}

template<>
void setupProperty(QTreeWidgetItem* item, const model::Vector3I32& vector){
	item->setText(TYPE_COLUMN, "VectorI32");
	item->setText(VALUE_COLUMN, QString("[%1; %2; %3]").arg(vector.x).arg(vector.y).arg(vector.z));
	addProperty(item, "x", vector.x);
	addProperty(item, "y", vector.y);
	addProperty(item, "z", vector.z);
}

template<>
void setupProperty(QTreeWidgetItem* item, const model::Vector3I8& vector){
	item->setText(TYPE_COLUMN, "Vector3I8");
	item->setText(VALUE_COLUMN, QString("[%1; %2; %3]").arg(vector.x).arg(vector.y).arg(vector.z));
	addProperty(item, "x", vector.x);
	addProperty(item, "y", vector.y);
	addProperty(item, "z", vector.z);
}


template<>
void setupProperty(QTreeWidgetItem* item, const model::Vector3F32& vector){
	item->setText(TYPE_COLUMN, "Vector3F32");
	item->setText(VALUE_COLUMN, QString("[%1; %2; %3]").arg(vector.x).arg(vector.y).arg(vector.z));
	addProperty(item, "x", vector.x);
	addProperty(item, "y", vector.y);
	addProperty(item, "z", vector.z);
}

template<>
void setupProperty(QTreeWidgetItem* item, const model::Vector3F64& vector){
	item->setText(TYPE_COLUMN, "Vector3F64");
	item->setText(VALUE_COLUMN, QString("[%1; %2; %3]").arg(vector.x).arg(vector.y).arg(vector.z));
	addProperty(item, "x", vector.x);
	addProperty(item, "y", vector.y);
	addProperty(item, "z", vector.z);
}

template<>
void setupProperty(QTreeWidgetItem* item, const model::Matrix3x3F64& property){
	item->setText(TYPE_COLUMN, "Matrix3x3F64");

	for(int i = 0; i < 9; i++){
		addProperty(item, QString("[%1]").arg(i), property.values[i]);
	}
}

template<>
void setupProperty(QTreeWidgetItem* item, const model::Vertex& property){
	item->setText(TYPE_COLUMN, "Vertex");
	addProperty(item, "tf", property.tf);
	addProperty(item, "pos", property.pos);
	addProperty(item, "sortInfo", property.sortInfo);
	item->setText(VALUE_COLUMN, QString("[%1; %2; %3]").arg(property.pos.x).arg(property.pos.y).arg(property.pos.z));
}

template<>
void setupProperty(QTreeWidgetItem* item, const model::Normal& property){
	item->setText(TYPE_COLUMN, "Normal");
	addProperty(item, "normal", property.normal);
	addProperty(item, "nPower", property.nPower);
	item->setText(VALUE_COLUMN, QString("[%1; %2; %3]").arg(property.normal.x).arg(property.normal.y).arg(property.normal.z));
}

template<>
void setupProperty(QTreeWidgetItem* item, const model::PolygonNormal& property){
	item->setText(TYPE_COLUMN, "PolygonNormal");
	addProperty(item, "normal", property.normal);
	addProperty(item, "nPower", property.nPower);
	addProperty(item, "sortInfo", property.sortInfo);
	item->setText(VALUE_COLUMN, QString("[%1; %2; %3]").arg(property.normal.x).arg(property.normal.y).arg(property.normal.z));
}


template<>
void setupProperty(QTreeWidgetItem* item, const model::PolygonIndex& property){
	item->setText(TYPE_COLUMN, "PolygonIndex");
	addProperty(item, "vertInd", property.vertInd);
	addProperty(item, "normInd", property.normInd);
	item->setText(VALUE_COLUMN, QString("[%1; %2]").arg(property.vertInd).arg(property.normInd));
}

template<>
void setupProperty(QTreeWidgetItem* item, const model::Polygon& property){
	item->setText(TYPE_COLUMN, property.num == 3 ?  "Polygon (Triangle)" : "Polygon (Quad)");
	addProperty(item, "num", property.num);
	addProperty(item, "sortInfo", property.sortInfo);
	addProperty(item, "colorId", property.colorId);
	addProperty(item, "colorShift", property.colorShift);
	addProperty(item, "flatNormal", property.flatNormal);
	addProperty(item, "middle", property.middle);
	addProperty(item, "indices", property.indices);
}


template<>
void setupProperty(QTreeWidgetItem* item, const model::C3D& c3d){
	item->setText(TYPE_COLUMN, "C3D");

	addProperty(item, "version", c3d.version);
	addProperty(item, "numVert", c3d.numVert);
	addProperty(item, "numNorm", c3d.numNorm);
	addProperty(item, "numPoly", c3d.numPoly);
	addProperty(item, "numVertTotal", c3d.numVertTotal);
	addProperty(item, "maxBound", c3d.maxBound);
	addProperty(item, "minBound", c3d.minBound);
	addProperty(item, "offsets", c3d.offsets);
	addProperty(item, "rmax", c3d.rmax);
	addProperty(item, "angle", c3d.angle);
	addProperty(item, "rcm", c3d.rcm);
	addProperty(item, "J", c3d.J);
	addProperty(item, "vectices", c3d.vectices);
	addProperty(item, "normals", c3d.normals);
	addProperty(item, "polygons", c3d.polygons);
	for(size_t i = 0; i < 3; i++ ){
		addProperty(item, QString("polyInds[%1]").arg(i), c3d.polyInds[i]);
	}

	item->setExpanded(true);
//
}

template<>
void setupProperty(QTreeWidgetItem* item, const model::Wheel& wheel){
	item->setText(TYPE_COLUMN, "Wheel");

	addProperty(item, "steer", wheel.steer);
	addProperty(item, "f", wheel.f);
	addProperty(item, "width", wheel.width);
	addProperty(item, "radius", wheel.radius);
	addProperty(item, "boundIndex", wheel.boundIndex);
	addProperty(item, "model", wheel.model);
}

template<>
void setupProperty(QTreeWidgetItem* item, const model::Slot& slot){
	item->setText(TYPE_COLUMN, "Slot");

	addProperty(item, "rSlots", slot.rSlots);
	addProperty(item, "locationAngleOfSlots", slot.locationAngleOfSlots);
}


template<>
void setupProperty(QTreeWidgetItem* item, const model::M3D& property){
	item->setText(TYPE_COLUMN, "M3D");

	addProperty(item, "body", property.body);
	addProperty(item, "bounds", property.bounds);
	addProperty(item, "rMax", property.rMax);
	addProperty(item, "nWheels", property.nWheels);
	addProperty(item, "nDebris", property.nDebris);
	addProperty(item, "bodyColorOffset", property.bodyColorOffset);
	addProperty(item, "bodyColorShift", property.bodyColorShift);
	addProperty(item, "wheels", property.wheels);
	addProperty(item, "debris", property.debris);
	addProperty(item, "boundDebris", property.boundDebris);
	addProperty(item, "bound", property.bound);
	addProperty(item, "slotsExistence", property.slotsExistence);
	addProperty(item, "rSlots", property.rSlots);
}

template<>
void setupProperty(QTreeWidgetItem* item, const model::A3D& property)
{
	addProperty(item, "nModels", property.nModels);
	addProperty(item, "bounds", property.bounds);
	addProperty(item, "rMax", property.rMax);
	addProperty(item, "bodyColorOffset", property.bodyColorOffset);
	addProperty(item, "bodyColorShift", property.bodyColorShift);
	addProperty(item, "models", property.models);
}
