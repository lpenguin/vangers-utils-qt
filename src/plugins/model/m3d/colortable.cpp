#include "colortable.h"

namespace vangers::model::m3d {
	const char* ObjectNames[(uint32_t)ObjectType::MAX_VALUE] = {
		"Body", "Debris", "Wheel", "Wheel (Static)", "Slot1", "Slot2", "Slot3", "ObjectBound", "DebreeBound", "Unknown"
	};


	const char* ColorNames[(uint32_t)ColorId::MAX_VALUE] = {
						 "ZERO_RESERVED",
						 "BODY",
						 "WINDOW",
						 "WHEEL",
						 "DEFENSE",
						 "WEAPON",
						 "TUBE",
						 "BODY_RED",
						 "BODY_BLUE",
						 "BODY_YELLOW",
						 "BODY_GRAY",
						 "YELLOW_CHARGED",
						 "MATERIAL_0",
						 "MATERIAL_1",
						 "MATERIAL_2",
						 "MATERIAL_3",
						 "MATERIAL_4",
						 "MATERIAL_5",
						 "MATERIAL_6",
						 "MATERIAL_7",
						 "BLACK",
						 "BODY_GREEN",
						 "SKYFARMER_KERNOBOO",
						 "SKYFARMER_PIPETKA",
						 "ROTTEN_ITEM",
						 "BODY_CRIMSON"
					 };


	uint8_t COLORS_VALUE_TABLE[2*(int)ColorId::MAX_VALUE] =
	{
		0,	0,				// 0 - ZERO_RESERVED,

		128, 	3,			      //1 - BODY,  changeable

		176, 	4,			      // 2 - WINDOW,
		224, 	7,			      // 3 - WHEEL,
		184, 	4,			      // 4 - DEFENSE,
		224, 	3,			      // 5 - WEAPON,
		224,	7,			      // 6 - TUBE,

		128, 	3,			      // 7 - BODY_RED,
		144, 	3,			      // 8 - BODY_BLUE,
		160, 	3,			      // 9 - BODY_YELLOW,
		228, 	4,			      // 10 - BODY_GRAY,
		112, 	4,			      // 11 - YELLOW_CHARGED,

		0,	2,				// 12 - MATERIAL_0,
		32,	2,			       // 13 - MATERIAL_1,
		64,	4,			       // 14 - MATERIAL_2,
		72,	3,			       // 15 - MATERIAL_3,
		88,	3,			       // 16 - MATERIAL_4,
		104,	4,			      // 17 - MATERIAL_5;
		112,	4,			      // 18 - MATERIAL_6,
		120,	4,			      // 19 - MATERIAL_7,
		184,	4,        		      // 20 - BLACK
		240,	3,		              // 21 - BODY_GREEN

		136,	4,				// 22 - SKYFARMER_KERNOBOO
		128,    4,				// 23 - SKYFARMER_PIPETKA
		224, 	4,			        // 24 - ROTTEN_ITEM

		192, 	3			        // 25 - BODY_CRIMSON
	};

	const int NORMAL_LEN = 7;
	const int NORMAL = ((1 << NORMAL_LEN) - 5);

	uint8_t get_color_index(uint32_t color_id)
	{
		uint8_t first_index= COLORS_VALUE_TABLE[2 * (int)color_id];
		uint8_t shift = COLORS_VALUE_TABLE[2 * (int)color_id + 1];
		uint8_t last_index = first_index + (NORMAL >> shift);

		float t = 0.9;
		return (first_index * (1 - t) + last_index * t);
	}
}


