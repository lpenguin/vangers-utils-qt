#include "vmap.h"
#include <QSharedPointer>
#include "../../image/palette.h"


QSharedPointer<QImage> imageFromData(const uint8_t* data, int sizeX, int sizeY, const vangers::Palette& palette){
	QSharedPointer<QImage> image = QSharedPointer<QImage>::create(data, sizeX, sizeY, sizeX, QImage::Format_Indexed8);
	image->setColorTable(palette);
	return image;
}

QSharedPointer<QImage> imageFromDataRed(const uint8_t* data, int sizeX, int sizeY){
	uint8_t* newData = new uint8_t[sizeof(uint32_t) * sizeX * sizeY];
	for(int i = 0; i < sizeX * sizeY; i++){
		newData[i * sizeof(uint32_t) + 0] = 0;
		newData[i * sizeof(uint32_t) + 1] = 0;
		newData[i * sizeof(uint32_t) + 2] = data[i];
		newData[i * sizeof(uint32_t) + 3] = 0;
	}
	// TODO: leak
	return QSharedPointer<QImage>::create(newData, sizeX, sizeY, sizeX * sizeof(uint32_t), QImage::Format_RGB32);
}


const Matrix<uint8_t> &Vmap::heightConst() const
{
	return _height;
}

Matrix<uint8_t>&  Vmap::height()
{
    return _height;
}

const Matrix<uint8_t> &Vmap::metaConst() const
{
	return _meta;
}

Matrix<uint8_t> &Vmap::meta()
{
	return _meta;
}

const std::vector<uint32_t>& Vmap::floodConst() const
{
	return _flood;
}

std::vector<uint32_t>& Vmap::flood()
{
	return _flood;
}


void Vmap::setPalette(const vangers::Palette &palette)
{
	_palette = palette;
}

void Vmap::setTerrainColorOffsets(const std::vector<std::pair<int, int> > &terrainColorOffsets)
{
	_terrainColorOffsets = terrainColorOffsets;
}

QSize Vmap::size() const
{
	return _size;
}

void Vmap::setSize(const QSize &size)
{
	_size = size;
}


const std::vector<std::pair<int, int>>& Vmap::terrainColorOffsetsConst() const
{
	return _terrainColorOffsets;
}

std::vector<std::pair<int, int> > &Vmap::terrainColorOffsets()
{
	return _terrainColorOffsets;
}

vangers::Palette& Vmap::palette()
{
	return _palette;
}

const vangers::Palette &Vmap::paletteConst() const
{
	return _palette;
}
