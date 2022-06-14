#ifndef MODELBINARYREADEREXT_H
#define MODELBINARYREADEREXT_H

#include <plugins/resourceaccess.h>
#include <binaryio.h>

#include <QIODevice>

#include "m3d.h"

template<>
bool vangers::BinaryReader::tryRead<vangers::model::m3d::C3D>(vangers::model::m3d::C3D& c3d);

template<>
bool vangers::BinaryReader::tryRead<vangers::model::m3d::M3D>(vangers::model::m3d::M3D &m3d);

template<>
bool vangers::BinaryReader::tryRead<vangers::model::m3d::A3D>(vangers::model::m3d::A3D &a3d);
#endif
