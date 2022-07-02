#ifndef RIFFENCODING_H
#define RIFFENCODING_H

#include "riff.h"

#include <QtCore/QBuffer>
#include <vangers/core//binaryio.h>

namespace riff {

	constexpr int32_t ChunkHeaderSize = sizeof (Chunk::id) +
										sizeof (Chunk::contentSize) +
										sizeof (Chunk::childrenSize);

	template<typename T>
	void encodeChunkContents(const T&, vangers::core::binario::BinaryWriter&, Chunk&){
		// NOOP
	}

	template<typename T>
	void encodeChunk(const T& t, Chunk& chunk){
		QBuffer buffer(&chunk.content);
		buffer.open(QBuffer::WriteOnly);
		vangers::core::binario::BinaryWriter writer(&buffer);
		encodeChunkContents(t, writer, chunk);
		buffer.close();
		chunk.contentSize = chunk.content.size();
		chunk.childrenSize = 0;
		for(const Chunk& childChunk: chunk.children){
			chunk.childrenSize += childChunk.contentSize + ChunkHeaderSize;
		}
	}

	template<typename T>
	void encodeChildChunk(const T& t, Chunk& parent){
		parent.children.append(Chunk{});
		encodeChunk(t, parent.children.last());
	}
}
#endif // RIFFENCODING_H
