#include "bmpimage.h"
#include "palette.h"

#include <QBuffer>



QSharedPointer<vangers::Image> vangers::BmpImageAccess::read(QFile &device)
{
    char buffer[256];
    device.read(buffer, 256);
    QByteArray data(buffer, 256);
    auto fileSize = device.size();
    device.seek(0);

    auto meta = _metaAccess.read(device);

    // Failed to find format
    if(!isValid(meta, fileSize)){
        return QSharedPointer<vangers::Image>();
    }

    const int sizeX = meta->value(vangers::ImageField::SizeX);
    const int sizeY = meta->value(vangers::ImageField::SizeY);
    int size = meta->value(vangers::ImageField::Size);
    size = size == 0 ? 1 : size;
    const int dataSize = sizeX * sizeY * size;
    char* buf = new char[dataSize];
    // TODO: check result
    qDebug() << "pos" << device.pos();
    qDebug() << "dataSize" << dataSize;
    int read = device.read(buf, dataSize);
    qDebug() << "read" << read;

    auto image = QSharedPointer<QImage>::create((uchar*) buf, sizeX, sizeY * size, sizeX, QImage::Format_Indexed8);

    image->setColorTable(vangers::Palette::grayscale());
    return QSharedPointer<vangers::Image>::create(image, meta);
}

void vangers::BmpImageAccess::write(const QSharedPointer<vangers::Image>& image, QFile& device)
{
    _metaAccess.write(image->meta(), device);
    auto qimage = image->image();
    int width = qimage->width();
    for(int iy = 0; iy < qimage->height(); iy++){
        const unsigned char* line = qimage->constScanLine(iy);
        device.write((char*)line, width);
    }

}

bool vangers::BmpImage1::isValid(const QSharedPointer<ImageMeta>& meta, qsizetype filesize)
{
    auto sizex = (*meta)[vangers::ImageField::SizeX];
    auto sizey = (*meta)[vangers::ImageField::SizeY];
    auto size = (*meta)[vangers::ImageField::Size];
    return filesize == (10 + size * sizex * sizey);
}

bool vangers::BmpImage2::isValid(const QSharedPointer<ImageMeta>& meta, qsizetype filesize)
{
    auto sizex = (*meta)[vangers::ImageField::SizeX];
    auto sizey = (*meta)[vangers::ImageField::SizeY];
    auto size = (*meta)[vangers::ImageField::Size];
    return filesize == (6 + size * sizex * sizey);
}

bool vangers::BmpImage3::isValid(const QSharedPointer<ImageMeta>& meta, qsizetype filesize)
{
    auto sizex = (*meta)[vangers::ImageField::SizeX];
    auto sizey = (*meta)[vangers::ImageField::SizeY];
    return filesize == (4 + sizex * sizey);
}

bool vangers::BmpImage4::isValid(const QSharedPointer<ImageMeta>& meta, qsizetype filesize)
{
    auto size = (*meta)[vangers::ImageField::Size];
    auto sizex = (*meta)[vangers::ImageField::SizeX];
    auto sizey = (*meta)[vangers::ImageField::SizeY];

    return filesize == (16 + size * sizex * sizey);
}
