#ifndef MSSIMAGEACCESS_H
#define MSSIMAGEACCESS_H

#include "image.h"


namespace vangers {
class MssImageAccess: public AbstractImageAccess
{
public:
    MssImageAccess();

    QSharedPointer<Image> read(QIODevice& file) override;
    void write(const QSharedPointer<Image>& image, QIODevice& file) override;
private:
    BinaryImageMetaAccess _metaAccess;
};

}

#endif // MSSIMAGEACCESS_H
