#ifndef VPRWRITER_H
#define VPRWRITER_H

#include <QIODevice>
#include <vangers/plugins/vmap/vmapparams.h>


namespace vangers::plugins::vmap {
	class VprWriter
	{
		public:
		void writeVpr(const VmapGlobalParams& globalParams, QIODevice& device);
	};

}


#endif // VPRWRITER_H
