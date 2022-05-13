#ifndef MODELOBJACCESS_H
#define MODELOBJACCESS_H
#include "model.h"

#include <QString>

namespace vangers {
	class ModelObjAccess
	{
	public:
		void write(const model::M3D& model, const QString& outputPath);
	};
}


#endif // MODELOBJACCESS_H
