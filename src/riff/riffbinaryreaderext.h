#ifndef RIFFBINARYREADEREXT_H
#define RIFFBINARYREADEREXT_H

#include <binaryio.h>
#include "riff.h"

template<>
bool vangers::BinaryReader::tryRead<vangers::riff::Riff>(vangers::riff::Riff& riff);

#endif // RIFFBINARYREADEREXT_H
