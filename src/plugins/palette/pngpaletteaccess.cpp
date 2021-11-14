#include "pngpaletteaccess.h"
#include <QImage>

bool vangers::PngPaletteAccess::read(Palette& palette, QIODevice &device)
{
    try {
        QFile& file = dynamic_cast<QFile&>(device);

        QImage image(file.fileName());

		palette.clear();

        for(int iy = 0; iy < image.height(); iy++){
            for(int ix = 0; ix < image.width(); ix ++){
				palette << image.pixel(ix, iy);
            }
        }
		return true;
    }  catch (std::bad_cast) {
		return false;
    }
}

void vangers::PngPaletteAccess::write(const Palette &palette, QIODevice &device)
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
