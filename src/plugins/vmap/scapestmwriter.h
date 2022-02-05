#ifndef SCAPESTMWRITER_H
#define SCAPESTMWRITER_H
#include "vmap.h"

#include <plugins/resourceaccess.h>

namespace vangers {
	class ScapeStmWriter
	{
	public:
		void write(const Vmap &resource, const QString& filename);
	private:
		void writeChunk(const Matrix<uint8_t>& resource, QIODevice& device, int32_t from, int32_t to);
	};
}


#endif // SCAPESTMWRITER_H
