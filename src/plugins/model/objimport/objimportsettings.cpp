#include "objimportsettings.h"

using namespace vangers::model::objimport;

template<typename K, typename V>
void mergeMap(const QMap<K,V>& src, QMap<K, V>& dst){
	for(const K& key: dst.keys()){
		if(src.contains(key)){
			dst[key] = src[key];
		}
	}
}

void ObjImportSettings::merge(const ObjImportSettings& src, ObjImportSettings& dst){
	mergeMap(src.objectMapping, dst.objectMapping);
	mergeMap(src.materialsMapping, dst.materialsMapping);
	dst.customScale = src.customScale;
	dst.customVolume = src.customVolume;
	dst.customRcm = src.customRcm;
	dst.customJ = src.customJ;
}
