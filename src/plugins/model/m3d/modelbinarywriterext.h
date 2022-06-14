#ifndef MODELBINARYWRITEREXT_H
#define MODELBINARYWRITEREXT_H
#include <plugins/resourceaccess.h>
#include <binaryio.h>

#include <QIODevice>

#include "m3d.h"

template<>
void vangers::BinaryWriter::write<vangers::model::m3d::M3D>(const vangers::model::m3d::M3D& m3d);


#endif // MODELBINARYWRITEREXT_H
