#ifndef SVSIMAGEACCESS_H
#define SVSIMAGEACCESS_H

#include "image.h"

namespace vangers {
class SvsImageAccess: public AbstractImageAccess
{
public:
    SvsImageAccess();

    QSharedPointer<Image> read(QIODevice& file) override;
    void write(const QSharedPointer<Image>& image, QIODevice& file) override;
private:
    BinaryImageMetaAccess _metaAccess;
};

}

#endif // SVSIMAGEACCESS_H
