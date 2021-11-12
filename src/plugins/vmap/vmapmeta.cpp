#include "vmapmeta.h"


VmapMeta VmapMeta::fromMeta(uint8_t meta)
{
	uint8_t delta = meta & 0b11;
	uint8_t objShadowFlag = (meta >> 2) & 0b1;
	uint8_t terrain = (meta >> 3) & 0b111;
	uint8_t doubleLevel = (meta >> 6) & 0b1;
	uint8_t shadowed = (meta >> 7) & 0b1;

	return VmapMeta(
				delta,
				objShadowFlag != 0,
				terrain,
				doubleLevel != 0,
				shadowed != 0
				);
}

uint8_t VmapMeta::terrain() const
{
	return _terrain;
}

uint8_t VmapMeta::delta() const
{
	return _delta;
}

bool VmapMeta::objectShadow() const
{
	return _objectShadow;
}

bool VmapMeta::isDoubleLevel() const
{
	return _isDoubleLevel;
}

bool VmapMeta::isShadowed() const
{
	return _isShadowed;
}
