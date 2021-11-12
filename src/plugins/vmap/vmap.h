#ifndef VMAP_H
#define VMAP_H

#include <stdint.h>
#include <vector>
#include <memory>
#include <QImage>
#include <image/palette.h>
#include "matrix.h"

class Vmap
{
public:
	Vmap(int sizeX, int sizeY,
		 const Matrix<uint8_t>& height,
		 const Matrix<uint8_t>& meta,
		 const vangers::Palette& palette,
		 const std::vector<std::pair<int, int>>& terrainColorOffsets)
        : _height(height)
        , _meta(meta)
        , _sizeX(sizeX)
        , _sizeY(sizeY)
		, _palette(palette)
		, _terrainColorOffsets(terrainColorOffsets)
    {}

	Matrix<uint8_t>& height();

	const Matrix<uint8_t> &meta() const;
	Matrix<uint8_t>& meta();
    QSharedPointer<QImage> heightImage();
    QSharedPointer<QImage> metaImage(uint8_t mask = 0b11111111);

	vangers::Palette palette() const;
	std::vector<std::pair<int, int>> terrainColorOffsets() const;
	int sizeX() const;
	int sizeY() const;

private:
	QSharedPointer<QImage> fromData(uint8_t *data);

	Matrix<uint8_t> _height;
	Matrix<uint8_t> _meta;

    int _sizeX;
    int _sizeY;

	vangers::Palette _palette;
	std::vector<std::pair<int, int>> _terrainColorOffsets;

};

#endif // VMAP_H
