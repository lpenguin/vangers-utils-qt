#ifndef MICROSOFTPALACCESS_H
#define MICROSOFTPALACCESS_H

#include "plugins/resourceaccess.h"
#include "image/palette.h"

#include <QSharedPointer>


namespace vangers {

class MicrosoftPalAccess: public AbstractResourceAccess<Palette>
{
public:
	bool read(Palette& palette, QIODevice &device) override;
	void write(const Palette &resource, QIODevice &device) override;

};

}

#endif // MICROSOFTPALACCESS_H
