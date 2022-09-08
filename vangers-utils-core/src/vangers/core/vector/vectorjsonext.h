#ifndef VECTORJSONEXT_H
#define VECTORJSONEXT_H

#include <nlohmann/json/json.hpp>
#include <vangers/core/vector/vector.h>

namespace vangers::core::vector {
	// Vector3<T>
	template<typename T>
	void to_json(nlohmann::json& j, const Vector3<T>& v) {
		j = nlohmann::json {v.x, v.y, v.z};
	}

	template<typename  T>
	void from_json(const nlohmann::json& j, Vector3<T>& v){
		j.at(0).get_to(v.x);
		j.at(1).get_to(v.y);
		j.at(2).get_to(v.z);
	}

	// Matrix3x3<T>
	template<typename T>
	void to_json(nlohmann::json& j, const Matrix3x3<T>& m) {
		j = m.values;
	}

	template<typename T>
	void from_json(const nlohmann::json& j, Matrix3x3<T>& m) {
		j.get_to(m.values);
	}
}

#endif // VECTORJSONEXT_H
