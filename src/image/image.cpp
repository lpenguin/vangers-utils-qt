#include "image.h"
#include "binaryio.h"

const QString HAS_EMBEDDED_PALETTE_FIELD("HasEmbeddedPalette");

QDebug operator <<(QDebug debug, vangers::ImageField field)
{
    return debug << toString(field);
}

bool vangers::BinaryImageMetaAccess::read(ImageMeta& meta, QIODevice &device)
{
    BinaryReader reader(&device);

    for (auto& pair : _readFormat) {
        auto field = pair.first;
        auto type = pair.second;
        quint32 data = 0;
        switch (type) {
            case vangers::FieldType::int16:
                data = reader.read<qint16>();
            break;
            case vangers::FieldType::uint16:
                data = reader.read<quint16>();
            break;
            case vangers::FieldType::uint32:
                data = reader.read<quint32>();
            break;
            case vangers::FieldType::int32:
                data = reader.read<qint32>();
            break;
        case vangers::FieldType::Invalid:
        default:
            continue;
        }
		meta.insertField(field, type, data);
    }

	return true;
}

void vangers::BinaryImageMetaAccess::write(const vangers::ImageMeta& meta, QIODevice &device)
{
    BinaryWriter writer(&device);
	for (auto& pair : meta.format()) {
        auto field = pair.first;
        auto type = pair.second;
		quint32 data = meta.value(field);
        switch (type) {
        case vangers::FieldType::uint16:
            writer.write((quint16)data);
        break;
        case vangers::FieldType::uint32:
            writer.write((quint32)data);
        break;
        case vangers::FieldType::int32:
            writer.write((qint32)data);
        break;
        case vangers::FieldType::Invalid:
        default:
            continue;
        }
    }
}

QString vangers::toString(vangers::ImageField field)
{
    switch (field) {
    case vangers::ImageField::Size:
        return "Size";
    case vangers::ImageField::SizeX:
        return "SizeX";
    case vangers::ImageField::SizeY:
        return "SizeY";
    case vangers::ImageField::BSizeX:
        return "BSizeX";
    case vangers::ImageField::BSizeY:
        return "BSizeY";
    case vangers::ImageField::OffsetX:
        return "OffsetX";
    case vangers::ImageField::OffsetY:
        return "OffsetY";
    case vangers::ImageField::Ctable:
        return "Ctable";
    case vangers::ImageField::Invalid:
    default:
        return "Invalid";

    }
}



bool vangers::IniImageMetaAccess::read(ImageMeta& meta, QIODevice &device)
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
        auto field = fromString<vangers::ImageField>(name);

        auto typeStr = nameType[1];
        auto valueStr = tokens[1];
        auto type = fromString<vangers::FieldType>(typeStr);
        quint32 value = 0;

        bool ok = false;
        switch (type) {
        case vangers::FieldType::uint16:
        case vangers::FieldType::uint32:
            value = valueStr.toUInt(&ok);
            break;
        case vangers::FieldType::int32:
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

void vangers::IniImageMetaAccess::write(const vangers::ImageMeta &meta, QIODevice &device)
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

QString vangers::toString(vangers::FieldType field)
{
    switch (field) {
    case vangers::FieldType::int32:
        return "int32";
    case vangers::FieldType::uint32:
        return "uint32";
    case vangers::FieldType::uint16:
        return "uint16";
    default:
        return "Invalid";
    }
}

template<>
vangers::FieldType vangers::fromString<vangers::FieldType>(const QString &field)
{
    if(field == "int32")
        return vangers::FieldType::int32;

    if(field == "uint32")
        return vangers::FieldType::uint32;

    if(field == "uint16")
        return vangers::FieldType::uint16;

    return vangers::FieldType::Invalid;
}

template<>
vangers::ImageField vangers::fromString<vangers::ImageField>(const QString &field)
{
    if(field == "Size")
        return vangers::ImageField::Size;

    if(field == "SizeX")
        return vangers::ImageField::SizeX;

    if(field == "SizeY")
        return vangers::ImageField::SizeY;

    if(field == "BSizeX")
        return vangers::ImageField::BSizeX;

    if(field == "BSizeY")
        return vangers::ImageField::BSizeY;

    if(field == "OffsetX")
        return vangers::ImageField::OffsetX;

    if(field == "OffsetY")
        return vangers::ImageField::OffsetY;

    if(field == "Ctable")
        return vangers::ImageField::Ctable;

    return vangers::ImageField::Invalid;
}


namespace vangers {
void Image::setImage(const QSharedPointer<QImage> &image)
{
    _image = image;
}

void Image::setMeta(const ImageMeta &meta)
{
    _meta = meta;
}

}
