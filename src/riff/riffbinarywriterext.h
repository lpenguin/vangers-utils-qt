#ifndef RIFFBINARYWRITEREXT_H
#define RIFFBINARYWRITEREXT_H

#include <riff/riff.h>
#include <binaryio.h>
#include <QBuffer>

namespace vangers {
	using namespace vangers::riff;

	template<>
	void vangers::BinaryWriter::write<Chunk>(const Chunk& chunk);

	template<>
	void vangers::BinaryWriter::write<Riff>(const Riff& riff);
}

#endif // RIFFBINARYWRITEREXT_H
