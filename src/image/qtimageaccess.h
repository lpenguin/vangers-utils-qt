#ifndef QTIMAGEACCESS_H
#define QTIMAGEACCESS_H

#include "image.h"



namespace vangers {
class QtImageAccess: public AbstractImageAccess
{
public:
    QtImageAccess(const QString& format)
        : _format(format)
    {}

	bool read(Image& image, QIODevice &file) override;
	void write(const Image &image, QIODevice &file) override;
private:
    IniImageMetaAccess _metaAccess;
    QString _format;
};
}

#endif // QTIMAGEACCESS_H
