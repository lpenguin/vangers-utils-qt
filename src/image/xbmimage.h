#ifndef XBMIMAGE_H
#define XBMIMAGE_H

#include "image.h"

namespace vangers {
class XbmImageAccess: public AbstractImageAccess
{
public:
    XbmImageAccess();

	bool read(Image& image, QIODevice& file) override;
	void write(const Image& image, QIODevice& file) override;
private:
    BinaryImageMetaAccess _metaAccess;
};

}

#endif // XBMIMAGE_H
