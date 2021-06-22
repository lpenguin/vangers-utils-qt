#include "bmpimage.h"
#include "palette.h"

#include <QBuffer>


using namespace vangers;

const ImageMetaFormat BmpVariant1 = {
    {vangers::ImageField::SizeX, vangers::FieldType::uint16},
    {vangers::ImageField::SizeY, vangers::FieldType::uint16},
    {vangers::ImageField::Size, vangers::FieldType::uint16},
    {vangers::ImageField::OffsetX, vangers::FieldType::uint16},
    {vangers::ImageField::OffsetY, vangers::FieldType::uint16},
};

const ImageMetaFormat BmpVariant2 =  {
    {vangers::ImageField::SizeX, vangers::FieldType::uint16},
    {vangers::ImageField::SizeY, vangers::FieldType::uint16},
    {vangers::ImageField::Size, vangers::FieldType::uint16},
};

const ImageMetaFormat BmpVariant3 = {
    {vangers::ImageField::SizeX, vangers::FieldType::uint16},
    {vangers::ImageField::SizeY, vangers::FieldType::uint16},
};

const ImageMetaFormat BmpVariant4 = {
    {vangers::ImageField::Size, vangers::FieldType::uint32},
    {vangers::ImageField::Ctable, vangers::FieldType::uint32},
    {vangers::ImageField::SizeX, vangers::FieldType::uint32},
    {vangers::ImageField::SizeY, vangers::FieldType::uint32},
};

bool isValid1(const ImageMeta& meta, qsizetype filesize)
{
    auto sizex = (meta)[vangers::ImageField::SizeX];
    auto sizey = (meta)[vangers::ImageField::SizeY];
    auto size = (meta)[vangers::ImageField::Size];
    return filesize == (10 + size * sizex * sizey);
}

bool isValid2(const ImageMeta& meta, qsizetype filesize)
{
    auto sizex = (meta)[vangers::ImageField::SizeX];
    auto sizey = (meta)[vangers::ImageField::SizeY];
    auto size = (meta)[vangers::ImageField::Size];
    return filesize == (6 + size * sizex * sizey);
}

bool isValid3(const ImageMeta& meta, qsizetype filesize)
{
    auto sizex = (meta)[vangers::ImageField::SizeX];
    auto sizey = (meta)[vangers::ImageField::SizeY];
    return filesize == (4 + sizex * sizey);
}

bool isValid4(const ImageMeta& meta, qsizetype filesize)
{
    auto size = (meta)[vangers::ImageField::Size];
    auto sizex = (meta)[vangers::ImageField::SizeX];
    auto sizey = (meta)[vangers::ImageField::SizeY];

    return filesize == (16 + size * sizex * sizey);
}

BmpImageAccess::BmpImageAccess()
    : _bmpVariants({
        {BmpVariant1, &isValid1},
        {BmpVariant2, &isValid2},
        {BmpVariant3, &isValid3},
        {BmpVariant4, &isValid4},
    })
{}

QSharedPointer<vangers::Image> vangers::BmpImageAccess::read(QIODevice &device)
{
    auto fileSize = device.size();

    for(const auto& formatIsValid: _bmpVariants){
        device.seek(0);

        BinaryImageMetaAccess access(formatIsValid.first);
        auto meta = access.read(device);
        if(formatIsValid.second(*meta, fileSize)){
            const int sizeX = meta->value(vangers::ImageField::SizeX);
            const int sizeY = meta->value(vangers::ImageField::SizeY);
            int size = meta->value(vangers::ImageField::Size);
            size = size == 0 ? 1 : size;
            const int dataSize = sizeX * sizeY * size;
            char* buf = new char[dataSize];
            // TODO: check result

            int read = device.read(buf, dataSize);
            auto image = QSharedPointer<QImage>::create((uchar*) buf, sizeX, sizeY * size, sizeX, QImage::Format_Indexed8);

            image->setColorTable(vangers::Palette::grayscale());
            return QSharedPointer<vangers::Image>::create(image, meta);
        }
    }

    return QSharedPointer<vangers::Image>();
}

void vangers::BmpImageAccess::write(const QSharedPointer<vangers::Image>& image, QIODevice& device)
{
    BinaryImageMetaAccess metaAccess(image->meta()->format());
    metaAccess.write(image->meta(), device);
    auto qimage = image->image();
    int width = qimage->width();
    for(int iy = 0; iy < qimage->height(); iy++){
        const unsigned char* line = qimage->constScanLine(iy);
        device.write((char*)line, width);
    }

}







