#include "riffbinarywriterext.h"

using namespace vangers;
using namespace vangers::riff;

template<>
void vangers::BinaryWriter::write<Chunk>(const Chunk& chunk){
	write(chunk.id, 4);
	write(chunk.contentSize);
	write(chunk.childrenSize);
	write(chunk.content);

	for(const Chunk& child: chunk.children){
		write(child);
	}
}

template<>
void vangers::BinaryWriter::write<Riff>(const Riff& riff)
{
	write(riff.id, 4);
	write<int32_t>(150);

	for(const Chunk& chunk: riff.chunks){
		write(chunk);
	}
}
