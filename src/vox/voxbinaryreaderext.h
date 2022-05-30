#ifndef VOXBINARYREADEREXT_H
#define VOXBINARYREADEREXT_H

#include <vox/vox.h>
#include <binaryio.h>

template<>
bool vangers::BinaryReader::tryRead(vangers::vox::Vox& vox);


#endif // VOXBINARYREADEREXT_H
