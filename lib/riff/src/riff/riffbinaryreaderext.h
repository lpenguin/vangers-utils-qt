#ifndef RIFFBINARYREADEREXT_H
#define RIFFBINARYREADEREXT_H

#include <vangers/core/binaryio.h>
#include "riff.h"

template<>
bool vangers::core::binario::BinaryReader::tryRead<riff::Riff>(riff::Riff& riff);

#endif // RIFFBINARYREADEREXT_H
