#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include <QImage>
#include <QVector>
#include <QMap>
#include <QIODevice>
#include <QDebug>
#include <QPair>
#include "image.h"

namespace vangers {

class BmpImageMetaValidator{
public:

};

typedef bool (*isValidPtr)(const ImageMeta& meta, qsizetype filezise);

class BmpImageAccess: public AbstractImageAccess
{
public:
    BmpImageAccess();

	bool read(Image& image, QIODevice& file) override;
	void write(const Image& image, QIODevice& file) override;
private:
    QList<QPair<ImageMetaFormat, isValidPtr>> _bmpVariants;
};



}


#endif // BMPIMAGE_H
