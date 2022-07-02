#ifndef RIFF_H
#define RIFF_H

#include <QByteArray>
#include <QList>
#include <cstdint>

namespace riff {
	struct Chunk {
		char id[4];
		int32_t contentSize;
		int32_t childrenSize;
		QByteArray content;
		QList<Chunk> children;

		void setId(const char* id);
		bool hasId(const char* id) const;

	};

	struct Riff {
		char id[4];
		int32_t version;
		QList<Chunk> chunks;

		void setId(const char* id);
	};


}
#endif // RIFF_H
