#include "vangerspaletteaccess.h"


QSharedPointer<vangers::Palette> vangers::VangersPaletteAccess::read(QIODevice &device)
{
    return QSharedPointer<Palette>::create(Palette::read(device));
}

void vangers::VangersPaletteAccess::write(const QSharedPointer<Palette> &resource, QIODevice &device)
{
    Palette::store(*resource, device);
}
