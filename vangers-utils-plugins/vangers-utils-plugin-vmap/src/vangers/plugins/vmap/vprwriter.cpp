#include "vprwriter.h"


void vangers::plugins::vmap::VprWriter::writeVpr(const VmapGlobalParams& globalParams, QIODevice& device)
{
	int32_t mapSizeX = 1 << globalParams.mapPowerX;
	int32_t mapSizeY = 1 << globalParams.mapPowerY;
	int32_t geoNetQuant = 1 << globalParams.geoNetPower;

	int32_t netSize = mapSizeX * mapSizeY / geoNetQuant / geoNetQuant;

	int32_t floodSize = (mapSizeY >> globalParams.sectionSizePower);
	int32_t floodOffset = 2 * 4 +
	                      1 * 4 +
	                      2 * 4 * 4 +
	                      2 * netSize +
	                      2 * globalParams.geoNetPower * sizeof(int32_t) +
	                      2 * floodSize * globalParams.geoNetPower * sizeof(int32_t);

	int32_t fileSize = floodOffset + floodSize  * sizeof(uint32_t);

	char* data = new char[fileSize];
	std::fill_n(data, fileSize, 0);

	device.write(data, fileSize);
	delete[] data;
	//			if(ff.size() != (int)(2*4 + (1 + 4 + 4)*4 + 2*net_size + 2*POWER*4 + 2*PART_MAX*POWER*4 + PART_MAX*4)) ErrH.Abort("Incorrect VPR size");

	//			ff.seek(2*4 + 1*4,XS_BEG);

	//			ff.seek(2*4*4,XS_CUR);

	//			ff.seek(2*net_size + 2*POWER*sizeof(int) + 2*PART_MAX*POWER*sizeof(int),XS_CUR);
	//			ff.read(FloodLvl,PART_MAX*sizeof(unsigned));

}
