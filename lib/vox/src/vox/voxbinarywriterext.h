#ifndef VOXBINARYWRITEREXT_H
#define VOXBINARYWRITEREXT_H
#include <vangers/core//binaryio.h>
#include <vox/vox.h>

template<>
void vangers::core::binario::BinaryWriter::write<vox::Vox>(const vox::Vox& vox);

#endif // VOXBINARYWRITEREXT_H
