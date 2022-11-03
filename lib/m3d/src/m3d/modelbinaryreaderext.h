#ifndef MODELBINARYREADEREXT_H
#define MODELBINARYREADEREXT_H

#include <vangers/core/plugin/resourceaccess.h>
#include <vangers/core/binaryio.h>

#include <QIODevice>

#include "m3d.h"

template<>
bool vangers::core::binario::BinaryReader::tryRead<m3d::C3D>(m3d::C3D& c3d);

template<>
bool vangers::core::binario::BinaryReader::tryRead<m3d::M3D>(m3d::M3D &m3d);

template<>
bool vangers::core::binario::BinaryReader::tryRead<m3d::A3D>(m3d::A3D &a3d);
#endif
