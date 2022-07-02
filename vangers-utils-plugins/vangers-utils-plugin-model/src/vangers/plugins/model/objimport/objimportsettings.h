#ifndef OBJIMPORTSETTINGS_H
#define OBJIMPORTSETTINGS_H

#include <QMap>
#include <QMultiMap>

#include <m3d/colortable.h>
#include <vangers/core/optional.h>
#include <vangers/core/vector/vector.h>

namespace vangers::plugins::model::objimport  {

	struct ObjImportSettings
	{
		QMap<QString, m3d::ObjectType> objectMapping;
		QMap<QString, m3d::ColorId> materialsMapping;
		bool triangulate;
		int32_t bodyColorOffset;
		int32_t bodyColorShift;

		vangers::core::optional::Optional<float> customScale;
		vangers::core::optional::Optional<float> customVolume;
		vangers::core::optional::Optional<vangers::core::vector::Vector3F64> customRcm;
		vangers::core::optional::Optional<vangers::core::vector::Matrix3x3F64> customJ;

		static void merge(const ObjImportSettings& src, ObjImportSettings& dst);
	};

}

#endif // OBJIMPORTSETTINGS_H
