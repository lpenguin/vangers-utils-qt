#ifndef MSSIMAGEACCESS_H
#define MSSIMAGEACCESS_H

#include "image.h"


namespace vangers {
class MssImageAccess: public AbstractImageAccess
{
public:
    MssImageAccess();

	bool read(Image& image, QIODevice& file) override;
	void write(const Image& image, QIODevice& file) override;
private:
    BinaryImageMetaAccess _metaAccess;
};

}

#endif // MSSIMAGEACCESS_H
