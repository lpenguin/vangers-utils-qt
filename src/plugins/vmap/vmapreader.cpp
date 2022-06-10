#include "vmapreader.h"
#include <QSharedPointer>
#include <splay/splay.h>
#include <QBuffer>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QSettings>
#include <binaryio.h>
#include <span.h>
#include <vector>

using namespace vangers;


bool _decodeCompressed(
		Matrix<uint8_t>& height,
		Matrix<uint8_t>& meta,
		QSize size,
		QIODevice& device){
	BinaryReader reader(&device);

    const int sizeX = size.width();
    const int sizeY = size.height();

	height.resize(sizeX, sizeY);
	meta.resize(sizeX, sizeY);

	qDebug() << "sizeX" << sizeX << "sizeY" << sizeY << "size" << height.size();

    std::vector<int32_t> offsets(sizeY);
    std::vector<int16_t> sizes(sizeY);

	for(int i = 0; i < sizeY; i++){
		offsets[i] = reader.read<int32_t>();
		sizes[i] = reader.read<int16_t>();
	}

	Splay splay(&device);
	std::vector<uint8_t> dataHeight(sizeX);
	dataHeight.resize(sizeX);
	std::vector<uint8_t> dataMeta(sizeX);
	dataMeta.resize(sizeX);


	for(int iRow = 0; iRow < sizeY; iRow++){
		if(!device.seek(offsets[iRow])){
			qWarning() << "Cannot seek to" << offsets[iRow];
			return false;
		}

		std::vector<uint8_t> bytes;
		if(!reader.tryReadArray(bytes, sizes[iRow])) return false;

		splay.expand(bytes, dataHeight, dataMeta);

		for(int iCol = 0; iCol < sizeX; iCol++){
			height.setData(iCol, iRow, dataHeight[iCol]);
			meta.setData(iCol, iRow, dataMeta[iCol]);
		}
	}
	qDebug() << "_decode size" << height.size() << meta.size();

	return true;
}

bool _decodeRaw(
		Matrix<uint8_t>& height,
		Matrix<uint8_t>& meta,
		QSize size,
		QIODevice& device){
	BinaryReader reader(&device);

	const int sizeX = size.width();
	const int sizeY = size.height();

	height.resize(sizeX, sizeY);
	meta.resize(sizeX, sizeY);

	qDebug() << "sizeX" << sizeX << "sizeY" << sizeY << "size" << height.size();

	std::vector<uint8_t> dataHeight(sizeX);
	dataHeight.resize(sizeX);
	std::vector<uint8_t> dataMeta(sizeX);
	dataMeta.resize(sizeX);


	for(int iRow = 0; iRow < sizeY; iRow++){
		reader.tryReadArray(dataHeight, sizeX);
		reader.tryReadArray(dataMeta, sizeX);

		for(int iCol = 0; iCol < sizeX; iCol++){
			height.setData(iCol, iRow, dataHeight[iCol]);
			meta.setData(iCol, iRow, dataMeta[iCol]);
		}
	}
	qDebug() << "_decode size" << height.size() << meta.size();

	return true;
}

bool _readOffsets(std::vector<std::pair<int, int>>&terrainColorOffsets,
				  const QSettings& settings){
	QStringList beginColorsStrs = settings.value("Rendering Parameters/Begin Colors")
			.toString()
			.trimmed()
			.split(QRegExp("\\s+"));

	if(beginColorsStrs.size() < 8){
		qWarning() << "Invalid begin colors string" << beginColorsStrs;
		return false;
	}

	QStringList endColorsStrs = settings.value("Rendering Parameters/End Colors")
			.toString()
			.trimmed()
			.split(QRegExp("\\s+"));

	if(endColorsStrs.size() < 8){
		qWarning() << "Invalid end colors string" << endColorsStrs;
		return false;
	}

	terrainColorOffsets.clear();

	for(int i = 0 ; i < beginColorsStrs.size(); i++)
	{
		int beginOffset = beginColorsStrs[i].toInt();
		int endOffset = endColorsStrs[i].toInt();
		terrainColorOffsets.push_back({beginOffset, endOffset});
	}
	return true;
}

bool _readPalette(vangers::Palette & palette, const QDir& fileDir, const QSettings& settings){
	QString paletteName = settings.value("Storage/Palette File").toString();
	QString paletteFileName = fileDir.filePath(paletteName);
	QFileInfo paletteFileInfo(paletteFileName);
	if(!paletteFileInfo.exists()){
		qWarning() << "Palette doesn't exist" << paletteFileName;
		return false;
	}

	QFile paletteFile(paletteFileName);
	paletteFile.open(QFile::ReadOnly);
	palette << vangers::Palette::read(paletteFile);
	paletteFile.close();
	return true;
}


bool _decodeFlood(std::vector<uint32_t>& flood, int64_t offset, int32_t size, QIODevice& device){
	flood.resize(size);
	QByteArray data = device.readAll();
	int64_t expectedFileSize = offset + (size * sizeof(int32_t));

	if(expectedFileSize != data.size()){
		qDebug() << "Invalid vpr file size, expected:"
				 << expectedFileSize
				 << ", actual:"
				 << data.size();

		return false;
	}

	QBuffer buffer(&data);
	if(!buffer.open(QBuffer::ReadOnly)){
		return false;
	}

	buffer.seek(offset);

	BinaryReader reader(&buffer);
	return reader.tryReadArray(flood, size);
}

bool _readData(
		Matrix<uint8_t>& height,
		Matrix<uint8_t>& meta,
		std::vector<uint32_t>& flood,
		QSize& size,
		const QDir& fileDir,
		const QSettings& settings){
	QString levelBaseName = settings.value("Storage/File Name").toString();

	bool isCompressed = settings.value("Storage/Compressed Format Using").toString().trimmed() == "1";
	int mapPowerX = settings.value("Global Parameters/Map Power X").toInt();
	int mapPowerY = settings.value("Global Parameters/Map Power Y").toInt();
	int geoNetPower = settings.value("Global Parameters/GeoNet Power").toInt();
	int sectionSizePower = settings.value("Global Parameters/Section Size Power").toInt();
	int minSquarePower  = settings.value("Global Parameters/Minimal Square Power").toInt();

	int sizeX = 1 << mapPowerX;
	int sizeY = 1 << mapPowerY;

	size.setWidth(sizeX);
	size.setHeight(sizeY);

	QString vmcFileName = fileDir.filePath(levelBaseName + (isCompressed ? ".vmc" : ".vmp"));
	QFileInfo vmcFileInfo = vmcFileName;

	if(!vmcFileInfo.exists()){
		qWarning() << "VMP/VMC file doesn't exsit" << vmcFileName;
		return false;
	}

	QFile vmcFile(vmcFileName);
	if(!vmcFile.open(QFile::ReadOnly)){
		qWarning() << "Cannot open file for reading" << vmcFileName;
		return false;
	}

	if(isCompressed)
		_decodeCompressed(height, meta, size, vmcFile);
	else
		_decodeRaw(height, meta, size, vmcFile);
	vmcFile.close();


	int floodSize = sizeY >> sectionSizePower;
	int netSize = (sizeX * sizeY) >> (2 * geoNetPower);

	int64_t floodOffset = (2 * 4 + (1 + 4 + 4) * 4 + 2 * netSize + 2 * geoNetPower * 4 + 2 * floodSize * geoNetPower * 4);

	QString vprFileName = fileDir.filePath(levelBaseName + ".vpr");
	QFileInfo vprFileInfo(vmcFileInfo);
	
	flood.resize(floodSize);
	std::fill(flood.begin(), flood.end(), 0);
	
	if(vprFileInfo.exists()) {
		QFile vprFile(vprFileName);
		if(vprFile.open(QFile::ReadOnly)) {
			_decodeFlood(flood, floodOffset, floodSize, vprFile));
		} else {
			qWarning() << "Cannot open file for reading" << vprFileName;
		}
	} else {
		qWarning() << "VPR file doesn't exsit" << vprFileName;
	}


	qDebug() << "_readData size" << height.size() << meta.size();
	return true;
}

bool VmapReader::read(Vmap& vmap, QFile &iniFile)
{
	QString filename = iniFile.fileName();

	QSettings settings(filename, QSettings::IniFormat);

	QFileInfo fileInfo(filename);
	QDir fileDir(fileInfo.absoluteDir());


	if(!_readPalette(vmap.palette(), fileDir, settings)){
		return false;
	}

	QSize size;
	if(!_readData(vmap.height(), vmap.meta(), vmap.flood(), size, fileDir, settings)){
		return false;
	}
	vmap.setSize(size);

	if(!_readOffsets(vmap.terrainColorOffsets(), settings)){
		return false;
	}

	qDebug() << "read size" << vmap.height().size() << vmap.height().size();

	qDebug() << vmap.height().size();
	qDebug() << vmap.meta().size();

//	const uint8_t* data = vmap.heightConst().data();

//	qDebug() << data[0] << data[1] << data[2];
	return true;
}
