#ifndef VLC_H
#define VLC_H

#include <string>
#include <variant>
#include <vector>
#include <vangers/core/vector/vector.h>

namespace vlc {
	struct SensorData {
		vangers::core::vector::Vector3I32 R_curr;
		int32_t radius;

		int32_t SensorType;
		std::string Name;
		vangers::core::vector::Vector3I32 vData;
		int32_t z0;
		int32_t Power;
		int32_t z1;
		int32_t data5;
		int32_t data6;
	};

	namespace {
		using Vlc = std::variant<std::vector<SensorData>>;
	}

}
#endif // VLC_H
