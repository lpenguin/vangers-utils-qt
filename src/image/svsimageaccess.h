#ifndef SVSIMAGEACCESS_H
#define SVSIMAGEACCESS_H

#include "image.h"

namespace vangers {
class SvsImageAccess: public AbstractImageAccess
{
public:
    SvsImageAccess();

	bool read(Image& image, QIODevice& file) override;
	void write(const Image& image, QIODevice& file) override;
private:
    BinaryImageMetaAccess _metaAccess;
};

}

#endif // SVSIMAGEACCESS_H
