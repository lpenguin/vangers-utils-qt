#ifndef VANGERSPALETTEACCESS_H
#define VANGERSPALETTEACCESS_H

#include "plugins/resourceaccess.h"
#include "image/palette.h"

#include <QSharedPointer>

namespace vangers {
class VangersPaletteAccess: public AbstractResourceAccess<Palette>
{
public:
    QSharedPointer<Palette> read(QIODevice &device) override;
    void write(const QSharedPointer<Palette> &resource, QIODevice &device) override;
};
}

#endif // VANGERSPALETTEACCESS_H
