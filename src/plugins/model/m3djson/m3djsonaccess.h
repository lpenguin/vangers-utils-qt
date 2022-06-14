#ifndef M3DJSONACCESS_H
#define M3DJSONACCESS_H

#include <plugins/model/m3d/m3d.h>
#include <plugins/resourceaccess.h>

namespace vangers::model::m3djson {
	using namespace vangers::model::m3d;

	class M3DJsonAccess: public vangers::AbstractResourceAccess<M3D>
	{
	public:
		bool read(M3D& resource, QIODevice& device);
		void write(const M3D& resource, QIODevice& device);

	};
}

#endif // M3DJSONACCESS_H
