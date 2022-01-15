#ifndef M3DADAPTER_H
#define M3DADAPTER_H
#import "model.h"
#include "drawdata.h"

#include <QVector>

class M3DAdapter
{
public:
	M3DAdapter();
	bool adapt(const model::M3D& m3d, QVector<DrawVBOData>& vertices, QVector<uint32_t>& indices);
};

#endif // M3DADAPTER_H
