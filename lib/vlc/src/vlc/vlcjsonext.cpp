#include "vlcjsonext.h"
#include <vangers/core/vector/vectorjsonext.h>

namespace vlc {
	void to_json(nlohmann::json& j, const SensorData& v){
		auto r_curr = nlohmann::json::object();
		vangers::core::vector::to_json(r_curr, v.R_curr);

		auto v_data = nlohmann::json::object();
		vangers::core::vector::to_json(v_data, v.vData);

		j = nlohmann::json {
		    {"R_curr", std::move(r_curr)},
		    {"radius", v.radius},
		    {"SensorType", v.SensorType},
		    {"Name", v.Name},
		    {"vData", std::move(v_data)},
		    {"z0", v.z0},
		    {"z1", v.z1},
		    {"Power", v.Power},
		    {"data5", v.data5},
		    {"data6", v.data6},
	    };
	}

	void to_json(nlohmann::json& j, const std::vector<SensorData>& v){
		j = nlohmann::json::array();
		for(const auto& s: v){
			auto obj = nlohmann::json();
			to_json(obj, s);
			j.push_back(std::move(obj));
		}
	}

	void from_json(const nlohmann::json& j, SensorData& v){
		// TODO:
	}

	void to_json(nlohmann::json& j, const Vlc& v)
	{
		if(std::holds_alternative<std::vector<vlc::SensorData>>(v)){

			const auto& vs = std::get<std::vector<SensorData>>(v);
			to_json(j, vs);
		}
	}

	void from_json(const nlohmann::json& j, Vlc& v)
	{
		// TODO:
	}

}

