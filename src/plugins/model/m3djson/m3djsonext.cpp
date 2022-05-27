#include <plugins/model/m3djson/m3djsonext.h>
#include <core/vector/vectorjsonext.h>

namespace vangers::model::m3d {
	void to_json(nlohmann::json& j, const Vertex& vertex) {
		j = nlohmann::json {
			{"tf", vertex.tf},
			{"pos", vertex.pos},
			{"sortInfo", vertex.sortInfo}
		};
	}

	void from_json(const nlohmann::json& j, Vertex& vertex){
		j.at("tf").get_to(vertex.tf);
		j.at("pos").get_to(vertex.pos);
		j.at("sortInfo").get_to(vertex.sortInfo);
	}

	void to_json(nlohmann::json& j, const Normal& normal) {
		j = nlohmann::json {
			{"nPower", normal.nPower},
			{"normal", normal.normal},
		};
	}

	void from_json(const nlohmann::json& j, Normal& normal){
		j.at("nPower").get_to(normal.nPower);
		j.at("normal").get_to(normal.normal);
	}

	void to_json(nlohmann::json& j, const PolygonNormal& normal) {
		j = nlohmann::json {
			{"nPower", normal.nPower},
			{"normal", normal.normal},
			{"sortInfo", normal.sortInfo},
		};
	}

	void from_json(const nlohmann::json& j, PolygonNormal& normal){
		j.at("nPower").get_to(normal.nPower);
		j.at("normal").get_to(normal.normal);
		j.at("sortInfo").get_to(normal.sortInfo);
	}

	void to_json(nlohmann::json& j, const PolygonIndex& index) {
		j = nlohmann::json {
			index.vertInd,
			index.normInd,
		};
	}

	void from_json(const nlohmann::json& j, PolygonIndex& index){
		j.at(0).get_to(index.vertInd);
		j.at(1).get_to(index.normInd);
	}

	void to_json(nlohmann::json& j, const Polygon& value) {
		j = nlohmann::json {
			{"num", value.num},
			{"sortInfo", value.sortInfo},
			{"colorId", value.colorId},
			{"colorShift", value.colorShift},
			{"flatNormal", value.flatNormal},
			{"middle", value.middle},
			{"indices", value.indices},
		};
	}

	void from_json(const nlohmann::json& j, Polygon& value){
		j.at("num").get_to(value.num);
		j.at("sortInfo").get_to(value.sortInfo);
		j.at("colorId").get_to(value.colorId);
		j.at("colorShift").get_to(value.colorShift);
		j.at("flatNormal").get_to(value.flatNormal);
		j.at("middle").get_to(value.middle);
		j.at("indices").get_to(value.indices);
	}

	void to_json(nlohmann::json& j, const C3D& value) {
		j = nlohmann::json {
			{"version", value.version},
			{"numVert", value.numVert},
			{"numNorm", value.numNorm},
			{"numPoly", value.numPoly},
			{"numVertTotal", value.numVertTotal},
			{"maxBound", value.maxBound},
			{"minBound", value.minBound},
			{"offsets", value.offsets},
			{"rmax", value.rmax},
			{"angle", value.angle},
			{"volume", value.volume},
			{"rcm", value.rcm},
			{"J", value.J},
			{"vectices", value.vectices},
			{"normals", value.normals},
			{"polygons", value.polygons},
			{"polyInds", value.polyInds},
		};
	}

	void from_json(const nlohmann::json& j, C3D& value){
		j.at("version").get_to(value.version);
		j.at("numVert").get_to(value.numVert);
		j.at("numNorm").get_to(value.numNorm);
		j.at("numPoly").get_to(value.numPoly);
		j.at("numVertTotal").get_to(value.numVertTotal);
		j.at("maxBound").get_to(value.maxBound);
		j.at("minBound").get_to(value.minBound);
		j.at("offsets").get_to(value.offsets);
		j.at("rmax").get_to(value.rmax);
		j.at("angle").get_to(value.angle);
		j.at("volume").get_to(value.volume);
		j.at("rcm").get_to(value.rcm);
		j.at("J").get_to(value.J);
		j.at("vectices").get_to(value.vectices);
		j.at("normals").get_to(value.normals);
		j.at("polygons").get_to(value.polygons);
		j.at("polyInds").get_to(value.polyInds);
	}

	void to_json(nlohmann::json& j, const Wheel& value) {
		j = nlohmann::json {
			{"steer", value.steer},
			{"f", value.f},
			{"width", value.width},
			{"radius", value.radius},
			{"boundIndex", value.boundIndex},
			{"model", value.model},
		};
	}

	void from_json(const nlohmann::json& j, Wheel& value){
		j.at("steer").get_to(value.steer);
		j.at("f").get_to(value.f);
		j.at("width").get_to(value.width);
		j.at("radius").get_to(value.radius);
		j.at("boundIndex").get_to(value.boundIndex);
		j.at("model").get_to(value.model);
	}

	void to_json(nlohmann::json& j, const Slot& value) {
		j = nlohmann::json {
			{"rSlots", value.rSlots},
			{"locationAngleOfSlots", value.locationAngleOfSlots},
		};
	}

	void from_json(const nlohmann::json& j, Slot& value){
		j.at("rSlots").get_to(value.rSlots);
		j.at("locationAngleOfSlots").get_to(value.locationAngleOfSlots);
	}

	void to_json(nlohmann::json& j, const M3D& value) {
		j = nlohmann::json {
			{"body", value.body},
			{"bounds", value.bounds},
			{"rMax", value.rMax},
			{"nWheels", value.nWheels},
			{"nDebris", value.nDebris},
			{"bodyColorOffset", value.bodyColorOffset},
			{"bodyColorShift", value.bodyColorShift},
			{"wheels", value.wheels},
			{"debris", value.debris},
			{"boundDebris", value.boundDebris},
			{"bound", value.bound},
			{"slotsExistence", value.slotsExistence},
			{"rSlots", value.rSlots},
		};
	}

	void from_json(const nlohmann::json& j, M3D& value){
		j.at("body").get_to(value.body);
		j.at("bounds").get_to(value.bounds);
		j.at("rMax").get_to(value.rMax);
		j.at("nWheels").get_to(value.nWheels);
		j.at("nDebris").get_to(value.nDebris);
		j.at("bodyColorOffset").get_to(value.bodyColorOffset);
		j.at("bodyColorShift").get_to(value.bodyColorShift);
		j.at("wheels").get_to(value.wheels);
		j.at("debris").get_to(value.debris);
		j.at("boundDebris").get_to(value.boundDebris);
		j.at("bound").get_to(value.bound);
		j.at("slotsExistence").get_to(value.slotsExistence);
		j.at("rSlots").get_to(value.rSlots);
	}

	void to_json(nlohmann::json& j, const A3D& value) {
		j = nlohmann::json {
			{"nModels", value.nModels},
			{"bounds", value.bounds},
			{"rMax", value.rMax},
			{"bodyColorOffset", value.bodyColorOffset},
			{"bodyColorShift", value.bodyColorShift},
			{"models", value.models},
		};
	}

	void from_json(const nlohmann::json& j, A3D& value){
		j.at("nModels").get_to(value.nModels);
		j.at("bounds").get_to(value.bounds);
		j.at("rMax").get_to(value.rMax);
		j.at("bodyColorOffset").get_to(value.bodyColorOffset);
		j.at("bodyColorShift").get_to(value.bodyColorShift);
		j.at("models").get_to(value.models);
	}

}
