#ifndef PNGIMAGE_H
#define PNGIMAGE_H
#include "image.h"


namespace vangers {
class PngImageAccess: public AbstractImageAccess
{
public:
    QSharedPointer<Image> read(QFile &file) override;
    void write(const QSharedPointer<Image> &image, QFile &file) override;
private:
    IniImageMetaAccess _metaAccess;
};

}

#endif // PNGIMAGE_H
