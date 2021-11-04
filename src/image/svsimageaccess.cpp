#include "palette.h"
#include "svsimageaccess.h"

#include <QBuffer>
#include <binaryio.h>

using namespace vangers;

const ImageMetaFormat SvsFormat = {
    {vangers::ImageField::SizeX, vangers::FieldType::uint16},
    {vangers::ImageField::SizeY, vangers::FieldType::uint16},
};

const quint32 PALETTE_SIZE = 768;

SvsImageAccess::SvsImageAccess():_metaAccess(SvsFormat)
{

}

QSharedPointer<QImage> _decode(QBuffer& buffer, const QByteArray& paletteBytes, quint32 width, quint32 height){
    size_t dataSize = width * height * sizeof(uint8_t);

    char* imageBytes = new char[dataSize];
    // TODO: check result

    int read = buffer.read(imageBytes, dataSize);
    if(read != dataSize){
        qWarning() <<"Image size mismatch: expected "<<dataSize<<", actual "<<read;
        return QSharedPointer<QImage>();
    }
    auto image = QSharedPointer<QImage>::create((uchar*) imageBytes, width, height, width, QImage::Format_Indexed8);

    Palette palette;
    for(int i = 0; i < paletteBytes.size(); i += 3){
        palette.append(qRgb(paletteBytes[i], paletteBytes[i+1], paletteBytes[i+2]) * 4);
    }
    image->setColorTable(palette);
    return image;
}

QSharedPointer<Image> SvsImageAccess::read(QIODevice &device)
{
    QByteArray paletteBytes = device.read(PALETTE_SIZE);
    auto meta = _metaAccess.read(device);
    meta->setHasEmbeddedPalette(true);
    QByteArray encoded = device.readAll();
    QBuffer buffer(&encoded);
    buffer.open(QBuffer::ReadOnly);


    quint32 width = (*meta)[vangers::ImageField::SizeX];
    quint32 height = (*meta)[vangers::ImageField::SizeY];

    quint32 filesize = width * height * sizeof (uint8_t);

    qint64 realSize = buffer.size();
    if(filesize != realSize){
        qWarning() << "Size mismatch"<<filesize << realSize;
        return QSharedPointer<vangers::Image>();
    }
    auto image =  _decode(buffer, paletteBytes, width, height);
    if(image.isNull()){
                return QSharedPointer<vangers::Image>();
    }
    return QSharedPointer<vangers::Image>::create(image, meta);
}

void SvsImageAccess::write(const QSharedPointer<Image> &image, QIODevice &device)
{
    BinaryImageMetaAccess metaAccess(image->meta()->format());
    BinaryWriter writer(&device);

    auto palette = image->image()->colorTable();
    if(palette.size() != 256){
        qWarning() << "Invalid palette size:"<<palette.size();
    }

    for(int i = 0; i < 256; i++){
        QRgb color = palette[i];
        writer.write<uint8_t>(qRed(color)/4);
        writer.write<uint8_t>(qGreen(color)/4);
        writer.write<uint8_t>(qBlue(color)/4);
    }

    metaAccess.write(image->meta(), device);
    auto qimage = image->image();
    int width = qimage->width();
    for(int iy = 0; iy < qimage->height(); iy++){
        const unsigned char* line = qimage->constScanLine(iy);
        device.write((char*)line, width);
    }
}
