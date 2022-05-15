#ifndef MODELACCESS_H
#define MODELACCESS_H

#include <plugins/resourceaccess.h>
#include "m3d.h"

namespace vangers::model::m3d {
	class M3DAccess : public vangers::AbstractResourceAccess<M3D>
	{
	public:

		void write(const M3D &resource, QIODevice &device);
		bool read(M3D &resource, QIODevice &device);
	};


	class A3DAccess : public vangers::AbstractResourceAccess<A3D>
	{
	public:

		void write(const A3D &resource, QIODevice &device);
		bool read(A3D &resource, QIODevice &device);
	};
}



#endif // MODELACCESS_H
