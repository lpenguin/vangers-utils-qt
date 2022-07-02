#ifndef MODELBINARYWRITEREXT_H
#define MODELBINARYWRITEREXT_H
#include <vangers/core//plugin/resourceaccess.h>
#include <vangers/core//binaryio.h>

#include <QIODevice>

#include "m3d.h"

template<>
void vangers::core::binario::BinaryWriter::write<m3d::M3D>(const m3d::M3D& m3d);


#endif // MODELBINARYWRITEREXT_H
