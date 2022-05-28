#ifndef OBJIMPORTSETTINGS_H
#define OBJIMPORTSETTINGS_H

#include <QMap>
#include <QMultiMap>

#include <plugins/model/m3d/colortable.h>

#include <core/optional.h>
#include <core/vector/vector.h>

namespace vangers::model::objimport {
	using namespace vangers::model::m3d;
	using namespace vangers::core::optional;
	using namespace vangers::core::vector;

	struct ObjImportSettings
	{
		QMap<QString, ObjectType> objectMapping;
		QMap<QString, ColorId> materialsMapping;
		bool triangulate;
		int32_t bodyColorOffset;
		int32_t bodyColorShift;

		Optional<float> customScale;
		Optional<float> customVolume;
		Optional<Vector3F64> customRcm;
		Optional<Matrix3x3F64> customJ;

		static void merge(const ObjImportSettings& src, ObjImportSettings& dst);
	};

}

#endif // OBJIMPORTSETTINGS_H
