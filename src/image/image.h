#ifndef IMAGE_H
#define IMAGE_H

#include <QVector>
#include <QIODevice>
#include <QDebug>
#include <QImage>
#include <QFile>
#include <QSharedPointer>

#include <plugins/resourceaccess.h>


namespace vangers {


enum class ImageField {
    SizeX,
    SizeY,
    BSizeX,
    BSizeY,
    Size,
    OffsetX,
    OffsetY,
    Ctable,
    Invalid
};

enum class FieldType {
    uint16,
    int16,
    uint32,
    int32,
    Invalid
};

typedef QVector<QPair<ImageField, FieldType>> ImageMetaFormat;

class BinaryImageMetaAccess;

class ImageMeta : public QMap<ImageField, quint32>{
public:
    ImageMeta():_hasEmbeddedPalette(false) {}
    ImageMeta(const ImageMetaFormat& format):_format(format),_hasEmbeddedPalette(false) {}

    void insertField(vangers::ImageField field, vangers::FieldType type, quint32 value) {
        insert(field, value);
        _format.append({field, type});
    }

    ImageMetaFormat& format() {
        return _format;
    }

    ImageMetaFormat format() const{
        return _format;
    }

    bool hasEmbeddedPalette() const {
        return _hasEmbeddedPalette;
    }

    void setHasEmbeddedPalette(bool value){
        _hasEmbeddedPalette = value;
    }
private:
    ImageMetaFormat _format;
    bool _hasEmbeddedPalette;
};

class AbstractImageMetaAccess: public AbstractResourceAccess<ImageMeta> {
};

class BinaryImageMetaAccess: public AbstractImageMetaAccess {
public:
    BinaryImageMetaAccess(const ImageMetaFormat readFormat):_readFormat(readFormat){}

	bool read(ImageMeta& meta, QIODevice &device) override;
	void write(const ImageMeta& meta, QIODevice& device) override;
private:
    ImageMetaFormat _readFormat;
};

class IniImageMetaAccess: public AbstractImageMetaAccess {
public:
	bool read(ImageMeta& meta, QIODevice &device) override;
	void write(const ImageMeta& meta, QIODevice& device) override;
};


class Image
{
public:
    QSharedPointer<QImage> image() const{
        return _image;
    }

	ImageMeta meta() const {
        return _meta;
    }

    bool hasEmbeddedPalette() const {
		return _meta.hasEmbeddedPalette();
    }

	void setImage(const QSharedPointer<QImage> &image);

	void setMeta(const ImageMeta &meta);

protected:
	QSharedPointer<QImage> _image;
	ImageMeta _meta;
};

class AbstractImageAccess: public AbstractResourceAccess<Image> {
};



QDebug operator << (QDebug debug, vangers::ImageField field);

QString toString(vangers::ImageField field);
QString toString(vangers::FieldType field);

template<typename T>
T fromString(const QString& field) {
    // noop
}

template<>
vangers::FieldType fromString<vangers::FieldType>(const QString& field);

template<>
vangers::ImageField fromString<vangers::ImageField>(const QString& field);



}

#endif // IMAGE_H
