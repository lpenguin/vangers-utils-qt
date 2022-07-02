#include "vangerspaletteaccess.h"

using namespace vangers::plugins::palette;

bool VangersPaletteAccess::read(Palette& palette, QIODevice &device)
{
	palette.clear();
	palette << Palette::read(device);
	return true;
}

void VangersPaletteAccess::write(const Palette &resource, QIODevice &device)
{
	Palette::store(resource, device);
}
