#include "vmap.h"
#include <QSharedPointer>
#include "../../image/palette.h"


std::vector<uint8_t>&  Vmap::height()
{
    return _height;
}

std::vector<uint8_t>& Vmap::meta()
{
    return _meta;
}

QSharedPointer<QImage> Vmap::fromData(uint8_t* data){
    QSharedPointer<QImage> image = QSharedPointer<QImage>::create(data, _sizeX, _sizeY, _sizeX, QImage::Format_Indexed8);
    image->setColorTable(vangers::Palette::grayscale());
    return image;
}

QSharedPointer<QImage> Vmap::heightImage()
{
    return fromData(_height.data());
}

QSharedPointer<QImage> Vmap::metaImage(uint8_t mask)
{
    uint8_t* metaMasked = new uint8_t[_sizeX * _sizeY];

    for(int i =0; i < _meta.size(); i++){
        uint8_t pixel = _meta[i];
        pixel &= mask;
        metaMasked[i] = pixel;
    }
    return fromData(metaMasked);
}
