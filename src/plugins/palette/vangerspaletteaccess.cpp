#include "vangerspaletteaccess.h"


bool vangers::VangersPaletteAccess::read(Palette& palette, QIODevice &device)
{
	palette.clear();
	palette << Palette::read(device);
	return true;
}

void vangers::VangersPaletteAccess::write(const Palette &resource, QIODevice &device)
{
	Palette::store(resource, device);
}
