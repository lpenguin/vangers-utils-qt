#include "riffbinaryreaderext.h"

#include <QBuffer>

using namespace vangers;
using namespace vangers::riff;


template<>
bool BinaryReader::tryRead<Chunk>(Chunk& chunk){
	if(!tryRead(chunk.id)) return false;
	if(!tryRead(chunk.contentSize)) return false;
	if(!tryRead(chunk.childrenSize)) return false;

	chunk.content = read(chunk.contentSize);
	qint64 childrenStartPos = _device->pos();
	qint64 childrenEndPos = childrenStartPos + chunk.childrenSize;
	while(_device->pos() < childrenEndPos){
		chunk.children.append(Chunk{});
		Chunk& child = chunk.children.last();
		if(!tryRead(child)) return false;
	}
	return true;
}

template<>
bool BinaryReader::tryRead<Riff>(Riff& riff)
{
	if(!tryRead(riff.id)) return false;
	if(!tryRead(riff.version)) return false;

	riff.chunks.append(Chunk{});
	Chunk& root = riff.chunks.last();
	if(!tryRead(root)) return false;

	return true;
}
