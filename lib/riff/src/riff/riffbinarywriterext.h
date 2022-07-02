#ifndef RIFFBINARYWRITEREXT_H
#define RIFFBINARYWRITEREXT_H

#include "riff.h"
#include <vangers/core//binaryio.h>
#include <QBuffer>

template<>
void vangers::core::binario::BinaryWriter::write<riff::Chunk>(const riff::Chunk& chunk);

template<>
void vangers::core::binario::BinaryWriter::write<riff::Riff>(const riff::Riff& riff);

#endif // RIFFBINARYWRITEREXT_H
