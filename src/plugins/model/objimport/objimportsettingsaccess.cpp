#include "objimportsettingsaccess.h"

#include <core/ini/iniaccess.h>
#include <core/vector/vectorstringext.h>

using namespace vangers::model::objimport;
using namespace vangers::core::ini;

const QString SectionObjects = "objects";
const QString SectionMaterials = "materials";
const QString SectionParameters = "parameters";

void ObjImportSettingsAccess::write(const ObjImportSettings& resource, QIODevice& device)
{
	IniSettings settings;
	for(const QString& name: resource.objectMapping.keys()){
		ObjectType type = resource.objectMapping[name];
		const char* typeName = ObjectNames[(int32_t)type];

		settings.setValue(SectionObjects, name, typeName);
	}

	for(const QString& name: resource.materialsMapping.keys()){
		ColorId colorId = resource.materialsMapping[name];
		const char* colorName = ColorNames[(int32_t)colorId];

		settings.setValue(SectionMaterials, name, colorName);
	}

	if(resource.customScale.hasValue()){
		settings.setValue(SectionParameters, "customScale", QString::number(resource.customScale.valueConst()));
	}

	if(resource.customVolume.hasValue()){
		settings.setValue(SectionParameters, "customVolume", QString::number(resource.customVolume.valueConst()));
	}

	if(resource.customRcm.hasValue()){
		settings.setValue(SectionParameters, "customRcm", toString(resource.customRcm.valueConst()));
	}

	if(resource.customJ.hasValue()){
		settings.setValue(SectionParameters, "customJ", toString(resource.customJ.valueConst()));
	}

	IniAccess access;
	access.write(settings, device);
}

bool findObjectType(ObjectType& type, const QString& name){
	for(int i = 0; i < (int32_t)ObjectType::MAX_VALUE; i++){
		if(name == ObjectNames[i]) {
			type = (ObjectType)i;
			return true;
		}
	}

	return false;
}

bool findColorId(ColorId& colorId, const QString& name){
	for(int i = 0; i < (int32_t)ColorId::MAX_VALUE; i++){
		if(name == ColorNames[i]) {
			colorId = (ColorId)i;
			return true;
		}
	}

	return false;
}

template<typename T>
bool readOptional(const IniSettings& settings, const QString& parameterName, Optional<T>& value){
	QString valueStr = settings.value(SectionParameters, parameterName);
	if(!valueStr.isNull() && !valueStr.isEmpty()){
		value = Optional<T>(T{});
		return fromString(valueStr, value.value());
	}
	value = Optional<T>();
	return true;
}

bool ObjImportSettingsAccess::read(ObjImportSettings& resource, QIODevice& device)
{
	IniAccess access;
	IniSettings settings;
	if(!access.read(settings, device)) return false;

	QStringList objectNames = settings.keys(SectionObjects);
	for(const QString& name: objectNames){
		QString typeString = settings.value(SectionObjects, name);
		ObjectType type;
		if(!findObjectType(type, typeString)) return false;

		resource.objectMapping[name] = type;
	}

	QStringList materialNames = settings.keys(SectionMaterials);
	for(const QString& name: materialNames){
		QString colorString= settings.value(SectionMaterials, name);
		ColorId colorId;
		if(!findColorId(colorId, colorString)) return false;

		resource.materialsMapping[name] = colorId;
	}

	if(!readOptional(settings, "customScale", resource.customScale)) {
		return false;
	}

	if(!readOptional(settings, "customVolume", resource.customVolume)) {
		return false;
	}

	if(!readOptional(settings, "customRcm", resource.customRcm)) {
		return false;
	}

	if(!readOptional(settings, "customJ", resource.customJ)) {
		return false;
	}

	return true;
}

