#ifndef JSONEXT_H
#define JSONEXT_H

#include <json.hpp>
#include <plugins/model/m3d/m3d.h>

namespace vangers::core::vector {
	// Vector3<T>
	template<typename T>
	void to_json(nlohmann::json& j, const Vector3<T>& v) {
		j = nlohmann::json {{"x", v.x}, {"y", v.y}, {"z", v.z}};
	}

	template<typename  T>
	void from_json(const nlohmann::json& j, Vector3<T>& v){
		j.at("x").get_to(v.x);
		j.at("y").get_to(v.y);
		j.at("z").get_to(v.z);
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
namespace vangers::model::m3d {
	// Vertex
	void to_json(nlohmann::json& j, const Vertex& vertex);
	void from_json(const nlohmann::json& j, Vertex& vertex);

	// Normal
	void to_json(nlohmann::json& j, const Normal& normal);
	void from_json(const nlohmann::json& j, Normal& normal);

	// PolygonNormal
	void to_json(nlohmann::json& j, const PolygonNormal& normal);
	void from_json(const nlohmann::json& j, PolygonNormal& normal);

	// PolygonIndex
	void to_json(nlohmann::json& j, const PolygonIndex& index);
	void from_json(const nlohmann::json& j, PolygonIndex& index);

	// Polygon
	void to_json(nlohmann::json& j, const Polygon& value);
	void from_json(const nlohmann::json& j, Polygon& value);

	// C3D
	void to_json(nlohmann::json& j, const C3D& value);
	void from_json(const nlohmann::json& j, C3D& value);


	// Wheel
	void to_json(nlohmann::json& j, const Wheel& value);
	void from_json(const nlohmann::json& j, Wheel& value);

	// Slot
	void to_json(nlohmann::json& j, const Slot& value);
	void from_json(const nlohmann::json& j, Slot& value);

	// M3D
	void to_json(nlohmann::json& j, const M3D& value);
	void from_json(const nlohmann::json& j, M3D& value);

	// A3D
	void to_json(nlohmann::json& j, const A3D& value);
	void from_json(const nlohmann::json& j, A3D& value);
}


#endif // JSONEXT_H
