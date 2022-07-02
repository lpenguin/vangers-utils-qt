#ifndef MICROSOFTPALACCESS_H
#define MICROSOFTPALACCESS_H

#include <vangers/core//plugin/resourceaccess.h>
#include <vangers/core//palette/palette.h>
#include <vangers/plugins/image/image.h>
#include <QSharedPointer>


namespace vangers::plugins::palette {
	using namespace vangers::core::plugin;
	using namespace vangers::core::palette;

class MicrosoftPalAccess: public AbstractResourceAccess<Palette>
{
public:
	bool read(Palette& palette, QIODevice &device) override;
	void write(const Palette &resource, QIODevice &device) override;

};

}

#endif // MICROSOFTPALACCESS_H
