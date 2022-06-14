#include "vmap.h"
#include <QSharedPointer>
#include "../../image/palette.h"


QSharedPointer<QImage> imageFromData(const uint8_t* data, int sizeX, int sizeY, const vangers::Palette& palette){
	QSharedPointer<QImage> image = QSharedPointer<QImage>::create(data, sizeX, sizeY, sizeX, QImage::Format_Indexed8);
	image->setColorTable(palette);
	return image;
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
