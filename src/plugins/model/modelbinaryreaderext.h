#ifndef MODELBINARYREADEREXT_H
#define MODELBINARYREADEREXT_H

#include <plugins/resourceaccess.h>
#include <binaryio.h>

#include <QIODevice>

#include "model.h"

template<>
bool vangers::BinaryReader::tryRead<model::C3D>(model::C3D& c3d);

#endif
