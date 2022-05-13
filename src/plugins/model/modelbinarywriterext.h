#ifndef MODELBINARYWRITEREXT_H
#define MODELBINARYWRITEREXT_H
#include <plugins/resourceaccess.h>
#include <binaryio.h>

#include <QIODevice>

#include "model.h"

template<>
void vangers::BinaryWriter::write<model::C3D>(const model::C3D& c3d);

//template<>
//void vangers::BinaryWriter::write<model::C3D>(const model::C3D& c3d);


#endif // MODELBINARYWRITEREXT_H
