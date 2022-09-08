#ifndef VLCJSONEXT_H
#define VLCJSONEXT_H

#include <nlohmann/json/json.hpp>
#include <vlc/vlc.h>

namespace vlc {
	void to_json(nlohmann::json& j, const vlc::Vlc& v);

	void from_json(const nlohmann::json& j, vlc::Vlc& v);
}
#endif // VLCJSONEXT_H
