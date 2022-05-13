#ifndef COLORTABLE_H
#define COLORTABLE_H

#include <inttypes.h>

enum class COLORS_IDS : uint32_t{
		ZERO_RESERVED,

		BODY,

		WINDOW,
		WHEEL,
		DEFENSE,
		WEAPON,
		TUBE,

		BODY_RED,
		BODY_BLUE,
		BODY_YELLOW,
		BODY_GRAY,
		YELLOW_CHARGED,

		MATERIAL_0,
		MATERIAL_1,
		MATERIAL_2,
		MATERIAL_3,
		MATERIAL_4,
		MATERIAL_5,
		MATERIAL_6,
		MATERIAL_7,

		BLACK,
		BODY_GREEN,

		SKYFARMER_KERNOBOO,
		SKYFARMER_PIPETKA,
		ROTTEN_ITEM,

		BODY_CRIMSON,

		MAX_COLORS_IDS
};

uint8_t get_color_index(uint32_t color_id);
#endif // COLORTABLE_H
