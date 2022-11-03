#ifndef MODELACCESS_H
#define MODELACCESS_H

#include <vangers/core/plugin/resourceaccess.h>
#include "m3d.h"

namespace m3d {
	class M3DAccess : public vangers::core::plugin::AbstractResourceAccess<M3D>
	{
	public:

		void write(const M3D &resource, QIODevice &device);
		bool read(M3D &resource, QIODevice &device);
	};


	class A3DAccess : public vangers::core::plugin::AbstractResourceAccess<A3D>
	{
	public:

		void write(const A3D &resource, QIODevice &device);
		bool read(A3D &resource, QIODevice &device);
	};
}



#endif // MODELACCESS_H
