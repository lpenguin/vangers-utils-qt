#include "xbmimage.h"

#include <vangers/core//binaryio.h>
#include <vangers/core//palette/palette.h>
#include <QtCore/QBuffer>
#include <QDebug>

using namespace vangers::core::binario;
using namespace vangers::core::palette;
using namespace vangers::plugins::image;

QByteArray _decode(QIODevice& device, quint32 width, quint32 height){
	auto reader = BinaryReader(&device);
    const unsigned char zero = 255;
    QByteArray screen = QByteArray(width * height, zero);

    quint32 count = reader.read<quint32>();
    while(count != 0 && device.pos() < device.size()){
        quint32 x = reader.read<quint32>();
        quint32 y = reader.read<quint32>();
        QByteArray bytes = reader.read(count);
        quint32 index = y * width + x;
        screen = screen.replace(index, count, bytes);
        qDebug() << "_readBlock" << x<<y<<count;
        count = reader.read<quint32>();
    }
    return screen;
}

void _writeBlock(BinaryWriter& writer, int xStart, int xEndExclusive, int y, const uchar* data){
    int count = xEndExclusive - xStart;
    qDebug() << "_writeBlock"<<xStart<<y<<count;
    writer.write<quint32>(count);
    writer.write<quint32>(xStart);
    writer.write<quint32>(y);
    writer.write(data + xStart, count);
}

void _encode(const QImage& image, QIODevice& device){
	BinaryWriter writer(&device);

    const uchar* data = image.bits();
    const unsigned char zero = 255;

    const int width = image.width();
    const int height = image.height();

    for(int iy = 0; iy < height; iy++){
        const uchar* line = image.constScanLine(iy);
        int ixStart = -1;
        for(int ix = 0; ix < width; ix++){
            // In empty region
            if(line[ix] == zero){
                if(ixStart != -1){
                    _writeBlock(writer, ixStart, ix, iy, line);
                    ixStart = -1;
                }
            } else { // In non empty region
                if(ixStart == -1){
                    ixStart = ix;
                }
            }
        }

        if(ixStart != -1){
            _writeBlock(writer, ixStart, width, iy, line);
        }
    }
    writer.write<qint32>(0);
    writer.write<qint32>(0);
}

XbmImageAccess::XbmImageAccess():_metaAccess({
	{ImageField::OffsetX, FieldType::uint32},
	{ImageField::OffsetY, FieldType::uint32},
	{ImageField::BSizeX, FieldType::uint32},
	{ImageField::BSizeY, FieldType::uint32},
	{ImageField::SizeX, FieldType::uint32},
	{ImageField::SizeY, FieldType::uint32},
	{ImageField::Size, FieldType::uint32},
})
{

}

bool XbmImageAccess::read(Image& image, QIODevice &device)
{
	ImageMeta meta;
	_metaAccess.read(meta, device);

    QByteArray encoded = device.readAll();
    QBuffer buffer(&encoded);
    buffer.open(QBuffer::ReadOnly);
	quint32 filesize = (meta)[ImageField::Size];
    qint64 realSize = buffer.size();
    if(filesize != realSize){
        qWarning() << "Size mismatch"<<filesize << realSize;
		return QSharedPointer<Image>();
    }

	quint32 width = (meta)[ImageField::SizeX];
	quint32 height = (meta)[ImageField::SizeY];
    QByteArray bytes = _decode(buffer, width, height);
    char* buf = new char[bytes.size()];
    memcpy(buf, bytes.constData(), bytes.size());
	auto img = QSharedPointer<QImage>::create(
                (uchar*) buf,
                width,
                height,
                width,
                QImage::Format_Indexed8);
	img->setColorTable(Palette::grayscale());

	image.setImage(img);
	image.setMeta(meta);
	return true;
}



void XbmImageAccess::write(const Image& image, QIODevice &device)
{
    QByteArray encodedArray;
    QBuffer encodedBuffer(&encodedArray);
    encodedBuffer.open(QBuffer::WriteOnly);
	_encode(*image.image(), encodedBuffer);
    encodedBuffer.close();

	auto meta = image.meta();
	meta.insert(ImageField::Size, encodedArray.size());
	_metaAccess.write(image.meta(), device);

    device.write(encodedArray);
}
