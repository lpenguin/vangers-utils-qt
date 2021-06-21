#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include <QImage>
#include <QVector>
#include <QMap>
#include <QIODevice>
#include <QDebug>
#include "image.h"

namespace vangers {


class BmpImageAccess: public AbstractImageAccess
{
public:
    BmpImageAccess(ImageMetaFormat format): _metaAccess(format){}

    QSharedPointer<Image> read(QIODevice& file) override;
    void write(const QSharedPointer<Image>& image, QIODevice& file) override;

    virtual bool isValid(const QSharedPointer<ImageMeta>& meta, qsizetype filesize) = 0;
private:
    BinaryImageMetaAccess _metaAccess;
};

class BmpImage1: public BmpImageAccess{
public:
    BmpImage1():BmpImageAccess( {
        {vangers::ImageField::SizeX, vangers::FieldType::uint16},
        {vangers::ImageField::SizeY, vangers::FieldType::uint16},
        {vangers::ImageField::Size, vangers::FieldType::uint16},
        {vangers::ImageField::OffsetX, vangers::FieldType::uint16},
        {vangers::ImageField::OffsetY, vangers::FieldType::uint16},
    })
    {}

    bool isValid(const QSharedPointer<ImageMeta>& meta, qsizetype filesize) override;
};

class BmpImage2: public BmpImageAccess{
public:
    BmpImage2():BmpImageAccess( {
        {vangers::ImageField::SizeX, vangers::FieldType::uint16},
        {vangers::ImageField::SizeY, vangers::FieldType::uint16},
        {vangers::ImageField::Size, vangers::FieldType::uint16},
    })
    {}

    bool isValid(const QSharedPointer<ImageMeta>& meta, qsizetype filesize) override;
};

class BmpImage3: public BmpImageAccess{
public:
    BmpImage3():BmpImageAccess( {
        {vangers::ImageField::SizeX, vangers::FieldType::uint16},
        {vangers::ImageField::SizeY, vangers::FieldType::uint16},
    })
    {}

    bool isValid(const QSharedPointer<ImageMeta>& meta, qsizetype filesize) override;
};

class BmpImage4: public BmpImageAccess{
public:
    BmpImage4():BmpImageAccess( {
        {vangers::ImageField::Size, vangers::FieldType::uint32},
        {vangers::ImageField::Ctable, vangers::FieldType::uint32},
        {vangers::ImageField::SizeX, vangers::FieldType::uint32},
        {vangers::ImageField::SizeY, vangers::FieldType::uint32},
    })
    {}

    bool isValid(const QSharedPointer<ImageMeta>& meta, qsizetype filesize) override;
};


}


#endif // BMPIMAGE_H
