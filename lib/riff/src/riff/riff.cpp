#include "riff.h"


void riff::Riff::setId(const char* id){
	::memcpy(this->id, id, sizeof(Riff::id));
}

void riff::Chunk::setId(const char* id){
	::memcpy(this->id, id, sizeof(Chunk::id));
}

bool riff::Chunk::hasId(const char* id) const
{
	return strncmp(this->id, id, sizeof(Chunk::id)) == 0;
}
