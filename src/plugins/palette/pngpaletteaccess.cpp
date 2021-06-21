#include "pngpaletteaccess.h"
#include <QImage>

QSharedPointer<vangers::Palette> vangers::PngPaletteAccess::read(QIODevice &device)
{
    try {
        QFile& file = dynamic_cast<QFile&>(device);

        QImage image(file.fileName());
        auto result = QSharedPointer<Palette>::create();

        Palette& pal = *result;

        for(int iy = 0; iy < image.height(); iy++){
            for(int ix = 0; ix < image.width(); ix ++){
                pal << image.pixel(ix, iy);
            }
        }
        return result;
    }  catch (std::bad_cast) {
        return QSharedPointer<vangers::Palette>();
    }
}

void vangers::PngPaletteAccess::write(const QSharedPointer<Palette> &resource, QIODevice &device)
{
    auto& palette = *resource;

    int nCols = 16;
    int nRows = palette.size() / nCols;
    QImage image(nCols, nRows, QImage::Format_RGB32);
    for(int i = 0; i < palette.size(); i++){
        int iCol = i % nCols;
        int iRow = i / nCols;
        image.setPixel(iCol, iRow, palette[i]);
    }
    image.save(&device, "png");
}
