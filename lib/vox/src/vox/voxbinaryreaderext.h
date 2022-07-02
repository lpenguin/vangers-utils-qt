#ifndef VOXBINARYREADEREXT_H
#define VOXBINARYREADEREXT_H

#include <vox/vox.h>
#include <vangers/core//binaryio.h>

template<>
bool vangers::core::binario::BinaryReader::tryRead(vox::Vox& vox);


#endif // VOXBINARYREADEREXT_H
