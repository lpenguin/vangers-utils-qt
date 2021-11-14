#include "vmapreader.h"
#include <QSharedPointer>
#include <splay/splay.h>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <binaryio.h>
#include <span.h>

using namespace vangers;


bool _decode(Matrix<uint8_t>& height, Matrix<uint8_t>& meta, QSize size, QIODevice& device){
	BinaryReader reader(&device);

	int sizeX = size.width();
	int sizeY = size.height();



	height.resize(sizeX, sizeY);
	meta.resize(sizeX, sizeY);

	qDebug() << "sizeX" << sizeX << "sizeY" << sizeY << "size" << height.size();

	int32_t offsets[sizeY];
	int16_t sizes[sizeY];

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

		std::vector<uint8_t> bytes = reader.readArray<uint8_t>(sizes[iRow]);
		splay.expand(bytes, dataHeight, dataMeta);

		for(int iCol = 0; iCol < sizeX; iCol++){
			height.setData(iCol, iRow, dataHeight[iCol]);
			meta.setData(iCol, iRow, dataMeta[iCol]);
		}
//		std::copy(dataHeight.data(), dataHeight.data() + sizeX, height.data() + (iRow * sizeX));
//		std::copy(dataMeta.data(), dataMeta.data() + sizeX, meta.data() + (iRow * sizeX));
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

bool _readData(Matrix<uint8_t>& height, Matrix<uint8_t>& meta, QSize& size, const QDir& fileDir, const QSettings& settings){
	QString levelBaseName = settings.value("Storage/File Name").toString();

	QString vmcFileName = fileDir.filePath(levelBaseName + ".vmc");
	QFileInfo vmcFileInfo = vmcFileName;

	if(!vmcFileInfo.exists()){
		qWarning() << "VMC file doesn't exsit" << vmcFileName;
		return false;
	}

	QFile vmcFile(vmcFileName);
	if(!vmcFile.open(QFile::ReadOnly)){
		qWarning() << "Cannot open file for reading" << vmcFileName;
		return false;
	}

	int mapPowerX = settings.value("Global Parameters/Map Power X").toInt();
	int mapPowerY = settings.value("Global Parameters/Map Power Y").toInt();

	int sizeX = 1 << mapPowerX;
	int sizeY = 1 << mapPowerY;

	size.setWidth(sizeX);
	size.setHeight(sizeY);
	_decode(height, meta, size, vmcFile);
	vmcFile.close();

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
	if(!_readData(vmap.height(), vmap.meta(), size, fileDir, settings)){
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
