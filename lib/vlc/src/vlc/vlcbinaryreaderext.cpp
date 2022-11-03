#include <vlc/vlcbinaryreaderext.h>

using namespace vangers::core::vector;
using namespace vlc;

template<>
bool vangers::core::binario::BinaryReader::tryRead<Vector3I32>(Vector3I32& data){
	if(!tryRead(data.x)) return false;
	if(!tryRead(data.y)) return false;
	if(!tryRead(data.z)) return false;
	return true;
}


template<>
bool vangers::core::binario::BinaryReader::tryRead<vlc::SensorData>(vlc::SensorData& data){
	if(!tryRead(data.R_curr)) return false;

	if(!tryRead(data.SensorType)) return false;

	if(!tryRead(data.radius)) return false;

	int n;
	if(!tryRead(n)) return false;

	if(!tryReadArray(data.Name, n)) return false;

	if(!tryRead(data.z0)) return false;

	if(!tryRead(data.vData)) return false;

	if(!tryRead(data.Power)) return false;

	if(!tryRead(data.z1)) return false;

	if(!tryRead(data.data5)) return false;

	if(!tryRead(data.data6)) return false;

	return true;
}

template<>
bool vangers::core::binario::BinaryReader::tryRead<vlc::Vlc>(vlc::Vlc& data)
{
	std::string header;
	if(!tryReadArray(header, 4)) return false;

	if(header == "VLS1"){
		int32_t nsns;
		if(!tryRead(nsns)) return false;

		std::vector<vlc::SensorData> s;

		for(int i = 0; i < nsns; i++){
			SensorData sd;
			if(!tryRead(sd)) return false;
			s.push_back(std::move(sd));
		}

		data = std::move(s);

		return true;
	}

	return false;
}
