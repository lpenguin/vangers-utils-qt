#ifndef VOXBINARYWRITEREXT_H
#define VOXBINARYWRITEREXT_H
#include <binaryio.h>
#include <vox/vox.h>

namespace vangers {
	using namespace vangers::vox;

	template<>
	void BinaryWriter::write<Vox>(const Vox& vox);
}

#endif // VOXBINARYWRITEREXT_H
