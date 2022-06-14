#include "bmpimage.h"

#include <QBuffer>
#include <vangers/core//palette/palette.h>

using namespace vangers::core::palette;
using namespace vangers::plugins::image;

const ImageMetaFormat BmpVariant1 = {
	{ImageField::SizeX, FieldType::uint16},
	{ImageField::SizeY, FieldType::uint16},
	{ImageField::Size, FieldType::uint16},
	{ImageField::OffsetX, FieldType::uint16},
	{ImageField::OffsetY, FieldType::uint16},
};

const ImageMetaFormat BmpVariant2 =  {
	{ImageField::SizeX, FieldType::uint16},
	{ImageField::SizeY, FieldType::uint16},
	{ImageField::Size, FieldType::uint16},
};

const ImageMetaFormat BmpVariant3 = {
	{ImageField::SizeX, FieldType::uint16},
	{ImageField::SizeY, FieldType::uint16},
};

const ImageMetaFormat BmpVariant4 = {
	{ImageField::Size, FieldType::uint32},
	{ImageField::Ctable, FieldType::uint32},
	{ImageField::SizeX, FieldType::uint32},
	{ImageField::SizeY, FieldType::uint32},
};

bool isValid1(const ImageMeta& meta, qsizetype filesize)
{
	auto sizex = (meta)[ImageField::SizeX];
	auto sizey = (meta)[ImageField::SizeY];
	auto size = (meta)[ImageField::Size];
    return filesize == (10 + size * sizex * sizey);
}

bool isValid2(const ImageMeta& meta, qsizetype filesize)
{
	auto sizex = (meta)[ImageField::SizeX];
	auto sizey = (meta)[ImageField::SizeY];
	auto size = (meta)[ImageField::Size];
    return filesize == (6 + size * sizex * sizey);
}

bool isValid3(const ImageMeta& meta, qsizetype filesize)
{
	auto sizex = (meta)[ImageField::SizeX];
	auto sizey = (meta)[ImageField::SizeY];
    return filesize == (4 + sizex * sizey);
}

bool isValid4(const ImageMeta& meta, qsizetype filesize)
{
	auto size = (meta)[ImageField::Size];
	auto sizex = (meta)[ImageField::SizeX];
	auto sizey = (meta)[ImageField::SizeY];

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

bool BmpImageAccess::read(Image& image, QIODevice &device)
{
    auto fileSize = device.size();

    for(const auto& formatIsValid: _bmpVariants){
        device.seek(0);

        BinaryImageMetaAccess access(formatIsValid.first);
		ImageMeta meta;
		access.read(meta, device);

		if(formatIsValid.second(meta, fileSize)){
			const int sizeX = meta.value(ImageField::SizeX);
			const int sizeY = meta.value(ImageField::SizeY);
			int size = meta.value(ImageField::Size);
            size = size == 0 ? 1 : size;
            const int dataSize = sizeX * sizeY * size;
            char* buf = new char[dataSize];
            // TODO: check result

            int read = device.read(buf, dataSize);
			auto img = QSharedPointer<QImage>::create((uchar*) buf, sizeX, sizeY * size, sizeX, QImage::Format_Indexed8);

			img->setColorTable(Palette::grayscale());
			image.setImage(img);
			image.setMeta(meta);
			return true;
        }
    }

	return false;
}

void BmpImageAccess::write(const Image& image, QIODevice& device)
{
	BinaryImageMetaAccess metaAccess(image.meta().format());
	metaAccess.write(image.meta(), device);
	auto qimage = image.image();
    int width = qimage->width();
    for(int iy = 0; iy < qimage->height(); iy++){
        const unsigned char* line = qimage->constScanLine(iy);
        device.write((char*)line, width);
    }

}







