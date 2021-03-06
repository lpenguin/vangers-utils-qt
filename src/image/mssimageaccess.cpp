#include "mssimageaccess.h"
#include "palette.h"

#include <QtCore/QBuffer>

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

bool MssImageAccess::read(Image& image, QIODevice &device)
{
	ImageMeta meta;
	_metaAccess.read(meta, device);
	meta.setHasEmbeddedPalette(true);
    QByteArray encoded = device.readAll();
    QBuffer buffer(&encoded);
    buffer.open(QBuffer::ReadOnly);


	quint32 size = (meta)[vangers::ImageField::Size];
	quint32 width = (meta)[vangers::ImageField::SizeX];
	quint32 height = (meta)[vangers::ImageField::SizeY];

    quint32 filesize = size * width * height * sizeof (int16_t);

    qint64 realSize = buffer.size();
    if(filesize != realSize){
        qWarning() << "Size mismatch"<<filesize << realSize;
		return false;
    }
	auto img = _decode(buffer, size, width, height);
	if(img.isNull()){
		return false;
    }

	image.setImage(img);
	image.setMeta(meta);
	return true;
}

void MssImageAccess::write(const Image &image, QIODevice &file)
{

}
