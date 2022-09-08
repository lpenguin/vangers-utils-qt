#ifndef VLCBINARYREADEREXT_H
#define VLCBINARYREADEREXT_H

#include <vlc/vlc.h>
#include <vangers/core/binaryio.h>


namespace vangers::core::binario {
	template<>
	bool BinaryReader::tryRead<vlc::Vlc>(vlc::Vlc& data);
}

#endif // VLCBINARYREADEREXT_H
