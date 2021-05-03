#ifndef IMAGE_H
#define IMAGE_H

#include <QVector>
#include <QIODevice>
#include <QDebug>
#include <QImage>
#include <QFile>
#include <QSharedPointer>


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
    uint32,
    int32,
    Invalid
};

typedef QVector<QPair<ImageField, FieldType>> ImageMetaFormat;

class BinaryImageMetaAccess;

class ImageMeta : public QMap<ImageField, quint32>{
public:
    ImageMeta() {}
    ImageMeta(const ImageMetaFormat& format):_format(format) {}

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

private:
    ImageMetaFormat _format;
};

class AbstractImageMetaAccess {
public:
    virtual QSharedPointer<ImageMeta> read(QIODevice &device) = 0;
    virtual void write(const QSharedPointer<ImageMeta>& meta, QIODevice& device) = 0;
    virtual ~AbstractImageMetaAccess() {}
};

class BinaryImageMetaAccess: public AbstractImageMetaAccess {
public:
    BinaryImageMetaAccess(const ImageMetaFormat readFormat):_readFormat(readFormat){}

    QSharedPointer<ImageMeta> read(QIODevice &device) override;
    void write(const QSharedPointer<ImageMeta>& meta, QIODevice& device) override;
private:
    ImageMetaFormat _readFormat;
};

class IniImageMetaAccess: public AbstractImageMetaAccess {
public:
    QSharedPointer<ImageMeta> read(QIODevice &device) override;
    void write(const QSharedPointer<ImageMeta>& meta, QIODevice& device) override;
};


class Image
{
public:
    Image(const QSharedPointer<QImage>& image, const QSharedPointer<ImageMeta>& meta): _image(image), _meta(meta){}

    QSharedPointer<QImage> image() const{
        return _image;
    }

    QSharedPointer<ImageMeta> meta() const {
        return _meta;
    }

protected:
    QSharedPointer<QImage> _image;
    QSharedPointer<ImageMeta> _meta;
};

class AbstractImageAccess {
public:
    virtual QSharedPointer<Image> read(QFile& file) = 0;
    virtual void write(const QSharedPointer<Image>& image, QFile& file) = 0;
    virtual ~AbstractImageAccess() {}
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
