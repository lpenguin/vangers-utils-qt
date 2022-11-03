#include "vmapparamsreader.h"

#include <QSettings>

void vangers::plugins::vmap::VmapParamsReader::readParams(const QString& fileName, VmapParams& params)
{
	QSettings settings(fileName, QSettings::IniFormat);

	params.storage.levelBaseName = settings.value("Storage/File Name").toString();
	params.storage.isCompressed = settings.value("Storage/Compressed Format Using").toString().trimmed() == "1";

	params.global.mapPowerX = settings.value("Global Parameters/Map Power X").toInt();
	params.global.mapPowerY = settings.value("Global Parameters/Map Power Y").toInt();
	params.global.geoNetPower = settings.value("Global Parameters/GeoNet Power").toInt();
	params.global.sectionSizePower = settings.value("Global Parameters/Section Size Power").toInt();
	params.global.minSquarePower  = settings.value("Global Parameters/Minimal Square Power").toInt();
}
