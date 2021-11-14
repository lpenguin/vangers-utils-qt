#ifndef PNGPALETTEACCESS_H
#define PNGPALETTEACCESS_H

#include "plugins/resourceaccess.h"
#include "image/palette.h"

#include <QSharedPointer>


namespace vangers {

class PngPaletteAccess: public AbstractResourceAccess<Palette>
{
public:
	bool read(Palette& palette, QIODevice &device) override;
	void write(const Palette &resource, QIODevice &device) override;
};

}


#endif // PNGPALETTEACCESS_H
