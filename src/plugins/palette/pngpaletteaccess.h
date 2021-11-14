#ifndef PNGPALETTEACCESS_H
#define PNGPALETTEACCESS_H

#include "plugins/resourceaccess.h"
#include "image/palette.h"

#include <QSharedPointer>


namespace vangers {

class PngPaletteAccess: public AbstractResourceAccess<Palette>
{
public:
	PngPaletteAccess(int nCols = 16)
		:_nCols(nCols)
	{}
	bool read(Palette& palette, QIODevice &device) override;
	void write(const Palette &resource, QIODevice &device) override;
private:
	int _nCols;
};

}


#endif // PNGPALETTEACCESS_H
