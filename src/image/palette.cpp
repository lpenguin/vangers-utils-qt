#include "palette.h"

#include <QFile>


vangers::Palette vangers::Palette::read(QIODevice &device)
{
    Palette result;
    auto data = device.readAll();
    for(int i = 0; i < data.size(); i += 3){
        result.append(qRgb(data[i], data[i+1], data[i+2]) * 4);
    }
    return result;

}

vangers::Palette vangers::Palette::read(const QString &paletteName)
{
    QFile f(QString(":/palettes/pal/") + paletteName);
    f.open(QFile::ReadOnly);
    return read(f);
}

vangers::Palette vangers::Palette::grayscale()
{
    Palette result;
    for(int i = 0; i < 256; i ++){
        result.append(qRgb(i, i, i));
    }
    return result;
}
