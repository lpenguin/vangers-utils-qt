#ifndef XBMIMAGE_H
#define XBMIMAGE_H

#include "image.h"

namespace vangers {
class XbmImageAccess: public AbstractImageAccess
{
public:
    XbmImageAccess();

    QSharedPointer<Image> read(QFile& file) override;
    void write(const QSharedPointer<Image>& image, QFile& file) override;
private:
    BinaryImageMetaAccess _metaAccess;
};

}

#endif // XBMIMAGE_H
