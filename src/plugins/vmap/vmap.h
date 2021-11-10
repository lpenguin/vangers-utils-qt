#ifndef VMAP_H
#define VMAP_H

#include <stdint.h>
#include <vector>
#include <memory>
#include <QImage>

class Vmap
{
public:
    Vmap(int sizeX, int sizeY, const std::vector<uint8_t>& height,
         const std::vector<uint8_t>& meta)
        : _height(height)
        , _meta(meta)
        , _sizeX(sizeX)
        , _sizeY(sizeY)
    {}

    std::vector<uint8_t>& height();

    std::vector<uint8_t>& meta();
    QSharedPointer<QImage> heightImage();
    QSharedPointer<QImage> metaImage(uint8_t mask = 0b11111111);

private:
    QSharedPointer<QImage> fromData(uint8_t *data);

    std::vector<uint8_t> _height;
    std::vector<uint8_t> _meta;

    int _sizeX;
    int _sizeY;

};

#endif // VMAP_H
