#include "image.h"
#include <vangers/core//binaryio.h>

const QString HAS_EMBEDDED_PALETTE_FIELD("HasEmbeddedPalette");

using namespace vangers::plugins::image;
using namespace vangers::core::binario;


QDebug vangers::plugins::image::operator <<(QDebug debug, ImageField field)
{
    return debug << toString(field);
}

bool BinaryImageMetaAccess::read(ImageMeta& meta, QIODevice &device)
{
    BinaryReader reader(&device);

    for (auto& pair : _readFormat) {
        auto field = pair.first;
        auto type = pair.second;
        quint32 data = 0;
        switch (type) {
			case FieldType::int16:
                data = reader.read<qint16>();
            break;
			case FieldType::uint16:
                data = reader.read<quint16>();
            break;
			case FieldType::uint32:
                data = reader.read<quint32>();
            break;
			case FieldType::int32:
                data = reader.read<qint32>();
            break;
		case FieldType::Invalid:
        default:
            continue;
        }
		meta.insertField(field, type, data);
    }

	return true;
}

void BinaryImageMetaAccess::write(const ImageMeta& meta, QIODevice &device)
{
    BinaryWriter writer(&device);
	for (auto& pair : meta.format()) {
        auto field = pair.first;
        auto type = pair.second;
		quint32 data = meta.value(field);
        switch (type) {
		case FieldType::uint16:
            writer.write((quint16)data);
        break;
		case FieldType::uint32:
            writer.write((quint32)data);
        break;
		case FieldType::int32:
            writer.write((qint32)data);
        break;
		case FieldType::Invalid:
        default:
            continue;
        }
    }
}

QString vangers::plugins::image::toString(ImageField field)
{
    switch (field) {
	case ImageField::Size:
        return "Size";
	case ImageField::SizeX:
        return "SizeX";
	case ImageField::SizeY:
        return "SizeY";
	case ImageField::BSizeX:
        return "BSizeX";
	case ImageField::BSizeY:
        return "BSizeY";
	case ImageField::OffsetX:
        return "OffsetX";
	case ImageField::OffsetY:
        return "OffsetY";
	case ImageField::Ctable:
        return "Ctable";
	case ImageField::Invalid:
    default:
        return "Invalid";

    }
}



bool IniImageMetaAccess::read(ImageMeta& meta, QIODevice &device)
{
	meta.setHasEmbeddedPalette(false);
    QTextStream st(&device);
    QString line;

    while(!(line = st.readLine()).isNull()){
        if(line == HAS_EMBEDDED_PALETTE_FIELD){
		   meta.setHasEmbeddedPalette(true);
           continue;
        }
        auto tokens = line.split(QRegExp("\\s*\\=\\s*"));
        if(tokens.size() != 2){
			return false;
        }
        auto nameType = tokens[0].split(QRegExp("\\s*\\:\\s*"));
        if(nameType.size() != 2){
			return false;
        }
        auto name = nameType[0];
		auto field = fromString<ImageField>(name);

        auto typeStr = nameType[1];
        auto valueStr = tokens[1];
		auto type = fromString<FieldType>(typeStr);
        quint32 value = 0;

        bool ok = false;
        switch (type) {
		case FieldType::uint16:
		case FieldType::uint32:
            value = valueStr.toUInt(&ok);
            break;
		case FieldType::int32:
            value = valueStr.toInt(&ok);
            break;
        default:
            ok = false;
        }
        if(!ok){
			return false;
        }

		meta.insertField(field, type, value);
    }
	return true;
}

void IniImageMetaAccess::write(const ImageMeta &meta, QIODevice &device)
{
    QTextStream st(&device);
	for(const auto& pair: meta.format()){
        auto field = pair.first;
        auto type = pair.second;
		auto value = meta.value(field);
        st << QString("%1:%2 = %3\r\n")
              .arg(toString(field))
              .arg(toString(type))
              .arg(value);
    }
	if(meta.hasEmbeddedPalette()){
        st << HAS_EMBEDDED_PALETTE_FIELD<< "\r\n";
    }
}

QString vangers::plugins::image::toString(FieldType field)
{
    switch (field) {
	case FieldType::int32:
        return "int32";
	case FieldType::uint32:
        return "uint32";
	case FieldType::uint16:
        return "uint16";
    default:
        return "Invalid";
    }
}

template<>
FieldType vangers::plugins::image::fromString<FieldType>(const QString &field)
{
    if(field == "int32")
		return FieldType::int32;

    if(field == "uint32")
		return FieldType::uint32;

    if(field == "uint16")
		return FieldType::uint16;

	return FieldType::Invalid;
}

template<>
ImageField vangers::plugins::image::fromString<ImageField>(const QString &field)
{
    if(field == "Size")
		return ImageField::Size;

    if(field == "SizeX")
		return ImageField::SizeX;

    if(field == "SizeY")
		return ImageField::SizeY;

    if(field == "BSizeX")
		return ImageField::BSizeX;

    if(field == "BSizeY")
		return ImageField::BSizeY;

    if(field == "OffsetX")
		return ImageField::OffsetX;

    if(field == "OffsetY")
		return ImageField::OffsetY;

    if(field == "Ctable")
		return ImageField::Ctable;

	return ImageField::Invalid;
}


void Image::setImage(const QSharedPointer<QImage> &image)
{
    _image = image;
}

void Image::setMeta(const ImageMeta &meta)
{
    _meta = meta;
}

