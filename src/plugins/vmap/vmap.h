#ifndef VMAP_H
#define VMAP_H

#include <stdint.h>
#include <vector>
#include <memory>
#include <QImage>
#include <image/palette.h>
#include "matrix.h"


QSharedPointer<QImage> imageFromData(const uint8_t* data, int sizeX, int sizeY, const vangers::Palette& palette);

class Vmap
{
public:
	Vmap()
		: _size({0, 0})
		, _height(0, 0)
		, _meta(0, 0)
	{}

	const Matrix<uint8_t>& heightConst() const;
	Matrix<uint8_t>& height();

	const Matrix<uint8_t> &metaConst() const;
	Matrix<uint8_t>& meta();

	vangers::Palette& palette();
	const vangers::Palette& paletteConst() const;
	const std::vector<std::pair<int, int>>& terrainColorOffsetsConst() const;
	std::vector<std::pair<int, int>>& terrainColorOffsets();

	QSize size() const;
	void setSize(const QSize &size);

	void setTerrainColorOffsets(const std::vector<std::pair<int, int> > &terrainColorOffsets);
	void setPalette(const vangers::Palette &palette);

private:
		QSize _size;

		Matrix<uint8_t> _height;
		Matrix<uint8_t> _meta;

	vangers::Palette _palette;
	std::vector<std::pair<int, int>> _terrainColorOffsets;

};

#endif // VMAP_H

