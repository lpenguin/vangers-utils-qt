#ifndef VMAPMETA_H
#define VMAPMETA_H

#include <stdint.h>

namespace vangers::plugins::vmap {
	class VmapMeta
	{

	public:
		static VmapMeta fromMeta(uint8_t meta);

		VmapMeta(const VmapMeta& other)
			: _delta(other._delta)
			, _objectShadow(other._objectShadow)
			, _terrain(other._terrain)
			, _isDoubleLevel(other._isDoubleLevel)
			, _isShadowed(other._isShadowed)
		{}

		uint8_t toMeta();

		uint8_t terrain() const;

		uint8_t delta() const;

		bool objectShadow() const;

		bool isDoubleLevel() const;

		bool isShadowed() const;

		VmapMeta(uint8_t delta, bool objectShadow, uint8_t terrain, bool isDoubleLevel, bool isShadowed)
			: _delta(delta)
			, _objectShadow(objectShadow)
			, _terrain(terrain)
			, _isDoubleLevel(isDoubleLevel)
			, _isShadowed(isShadowed)
		{}

		uint8_t _delta;
		bool _objectShadow;
		uint8_t _terrain;
		bool _isDoubleLevel;
		bool _isShadowed;
	};

}

#endif // VMAPMETA_H
