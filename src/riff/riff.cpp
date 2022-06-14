#include "riff.h"


void vangers::riff::Riff::setId(const char* id){
	::memcpy(this->id, id, sizeof(Riff::id));
}

void vangers::riff::Chunk::setId(const char* id){
	::memcpy(this->id, id, sizeof(Chunk::id));
}

bool vangers::riff::Chunk::hasId(const char* id) const
{
	return strncmp(this->id, id, sizeof(Chunk::id)) == 0;
}
