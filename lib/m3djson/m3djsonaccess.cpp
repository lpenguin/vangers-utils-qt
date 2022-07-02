#include "m3djsonaccess.h"

#include <vangers/core/vector/vectorjsonext.h>
#include <plugins/model/m3djson/m3djsonext.h>
#include <json.hpp>

using namespace vangers::model::m3djson;

bool M3DJsonAccess::read(M3D& resource, QIODevice& device)
{
	QByteArray text = device.readAll();
	nlohmann::json doc = nlohmann::json::parse(text.begin(), text.end());
	from_json(doc, resource);
	return true;
}

void M3DJsonAccess::write(const M3D& resource, QIODevice& device)
{
	nlohmann::json doc;
	to_json(doc, resource);
	auto d = doc.dump(2);
	device.write(d.c_str());
}
