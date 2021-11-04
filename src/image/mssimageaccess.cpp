#include "mssimageaccess.h"
#include "palette.h"

#include <QBuffer>

using namespace vangers;

const quint32 PALETTE_SIZE = 768;

const ImageMetaFormat MssFormat = {
    {vangers::ImageField::Size, vangers::FieldType::int32},
    {vangers::ImageField::SizeX, vangers::FieldType::int16},
    {vangers::ImageField::SizeY, vangers::FieldType::int16},
    {vangers::ImageField::OffsetX, vangers::FieldType::int16},
    {vangers::ImageField::OffsetY, vangers::FieldType::int16},

};

MssImageAccess::MssImageAccess():_metaAccess(MssFormat)
{

}

QSharedPointer<QImage> _decode(QBuffer& buffer, quint32 size, quint32 width, quint32 height){
    int dataSize = size * width * height * sizeof(int16_t);

    char* imageBytes = new char[dataSize];

    int read = buffer.read(imageBytes, dataSize);
    if(read != dataSize){
        qWarning() <<"Image size mismatch: expected "<<dataSize<<", actual "<<read;
        return QSharedPointer<QImage>();
    }
    auto image = QSharedPointer<QImage>::create((uchar*) imageBytes, width, height * size, width * sizeof(int16_t), QImage::Format_RGB16);

//    Palette palette;
//    for(int i = 0; i < 256; i ++){
//        palette.append(qRgb(i, i, i));
//    }
//    image->setColorTable(palette);
    return image;
}

QSharedPointer<Image> MssImageAccess::read(QIODevice &device)
{
    auto meta = _metaAccess.read(device);
    meta->setHasEmbeddedPalette(true);
    QByteArray encoded = device.readAll();
    QBuffer buffer(&encoded);
    buffer.open(QBuffer::ReadOnly);


    quint32 size = (*meta)[vangers::ImageField::Size];
    quint32 width = (*meta)[vangers::ImageField::SizeX];
    quint32 height = (*meta)[vangers::ImageField::SizeY];

    quint32 filesize = size * width * height * sizeof (int16_t);

    qint64 realSize = buffer.size();
    if(filesize != realSize){
        qWarning() << "Size mismatch"<<filesize << realSize;
        return QSharedPointer<vangers::Image>();
    }
    auto image =  _decode(buffer, size, width, height);
    if(image.isNull()){
        return QSharedPointer<vangers::Image>();
    }
    return QSharedPointer<vangers::Image>::create(image, meta);
}

void MssImageAccess::write(const QSharedPointer<Image> &image, QIODevice &file)
{

}
