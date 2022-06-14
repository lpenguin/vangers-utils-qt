#include "palette.h"

#include <QDir>
#include <QFile>
#include <QVector3D>
#include <QStandardPaths>
#include <QDebug>

using namespace vangers::core::palette;

const int  FIRE_COLOR_FIRST = 192;
const int  FIRE_COLOR_BITS = 5;

const int  FIRE_PROCESS_COLOR_MAX = 1 << FIRE_COLOR_BITS;

const int  WHITE_POINT = 1 << FIRE_COLOR_BITS;
const int  RED_POINT = WHITE_POINT/3;
const int  YELLOW_POINT = RED_POINT*2;

Palette Palette::read(QIODevice &device)
{
    Palette result;
    auto data = device.readAll();
    int size = qMin(768, data.size());
    for(int i = 0; i < size; i += 3){
        result.append(qRgb(data[i], data[i+1], data[i+2]) * 4);
    }
    return result;

}

Palette Palette::read(const QString &paletteName)
{
    if(paletteName == "~fire"){
		return Palette::fireball(fire(), 0);
    }

    if(paletteName.startsWith("custom/")){
        QDir dataLocation = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).filePath("custom");
        auto paletteFile = dataLocation.filePath(paletteName.mid(QString("custom/").length()));
        qDebug() << paletteFile;
        QFileInfo info(paletteFile);
        if(!info.exists()){
            return {};
        }
        QFile f(paletteFile);
        f.open(QFile::ReadOnly);
        return read(f);
    }

    QFile f(QString(":/palettes/pal/") + paletteName);
    f.open(QFile::ReadOnly);
    return read(f);
}

void Palette::store(const Palette& palette, const QString& name)
{
    QDir dataLocation = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).filePath("custom");
    qDebug() << dataLocation;
    if(!dataLocation.exists()){
        qDebug() << dataLocation.mkpath(".");
    }

    QString path = dataLocation.filePath(name);
    QFile f(path);
    f.open(QFile::WriteOnly);
    store(palette, f);
}

void Palette::store(const Palette& palette, QIODevice& device)
{
    for(auto& color: palette){
        char data[3];
        data[0] = qRed(color) / 4;
        data[1] = qGreen(color) / 4;
        data[2] = qBlue(color) / 4;
        device.write(data, 3);
    }
}

QStringList Palette::paletteNames()
{
    QStringList result = QDir(":/palettes/pal").entryList();
    result << "~fire";

    QDir dataLocation = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).filePath("custom");
    if(dataLocation.exists()){
        for(auto& entry: dataLocation.entryList(QDir::Files)){
            result << "custom/"+entry;
        }

    }
    return result;
}

Palette Palette::grayscale()
{
    Palette result;
    for(int i = 0; i < 256; i ++){
        result.append(qRgb(i, i, i));
    }
    return result;
}


// Borrowed from Vangers source code: src/particle/partmap.cpp
void FirePaletteInit(unsigned char* pal)
{
    int first_col = FIRE_COLOR_FIRST;
    int i;
    const int NULL_INTENS = 20;
    for(i = 0;i < RED_POINT;i++){
        pal[3*(i + first_col)] = NULL_INTENS + (64 - NULL_INTENS)*i/RED_POINT;
        pal[3*(i + first_col) + 1] = 0;
        pal[3*(i + first_col) + 2] = 0;
        }
    for(;i < YELLOW_POINT;i++){
        pal[3*(i + first_col)] = 63;
        pal[3*(i + first_col) + 1] = (64*(i - YELLOW_POINT)/(YELLOW_POINT - RED_POINT)) & 63;
        pal[3*(i + first_col) + 2] = 0;
        }
    for(;i < WHITE_POINT;i++){
        pal[3*(i + first_col)] = 63;
        pal[3*(i + first_col) + 1] = 63;
        pal[3*(i + first_col) + 2] = (64*(i - WHITE_POINT)/(WHITE_POINT - YELLOW_POINT)) & 63;
        }
    i = WHITE_POINT - 1;
    pal[3*(i + first_col)] = pal[3*(i + first_col) + 1] = pal[3*(i + first_col) + 2] = 63;
}

void FireTableInit(unsigned char* FireColorTable ){
    unsigned char palbuf[768];

    memset(palbuf, 0, 768);

    FirePaletteInit(palbuf);

    QVector3D vColor;

    vColor = QVector3D(63,63,63);
    int MaxVector = (int)vColor.length();

    for(int k = 0;k < 256;k++){
        for(int i = 0;i < 256;i++){
            vColor = QVector3D(palbuf[i*3], palbuf[i*3 + 1], palbuf[i*3 + 2]);

            int d1 = (int)vColor.length() + (MaxVector - (int)vColor.length()) * k / 256;

            int ti = FIRE_COLOR_FIRST;
            vColor = QVector3D(palbuf[FIRE_COLOR_FIRST*3],palbuf[FIRE_COLOR_FIRST*3 + 1],palbuf[FIRE_COLOR_FIRST*3 + 2]);
            int dec = abs((int)vColor.length() - d1);

            for(int j = FIRE_COLOR_FIRST + 1;j < FIRE_COLOR_FIRST + FIRE_PROCESS_COLOR_MAX;j++){
                vColor = QVector3D(palbuf[j*3],palbuf[j*3 + 1],palbuf[j*3 + 2]);
                int d2 = (int)vColor.length();
                int dc = abs(d2 - d1);
                if(dc < dec){
                    dec = dc;
                    ti = j;
                };
            };
            FireColorTable[i + (k << 8)] = ti;
        };
    };
}
Palette Palette::fire()
{
    unsigned char* pal = new unsigned char[768];
    memset(pal, 0, 768);
    FirePaletteInit(pal);
    Palette result;
    result.reserve(256);
    for(int i = 0; i < 256; i++){
        result.append(qRgb(pal[i * 3 + 0], pal[i * 3 + 1], pal[i * 3 + 2]) * 4);
    }
    return result;
}

Palette Palette::fireball(const Palette& basePalette, unsigned char baseColor)
{
    unsigned char FireColorTable[256 << 8];
    memset(FireColorTable, 0, 256 << 8);

    FireTableInit(FireColorTable);
	Palette result;
    for(int i = 0; i < 256; i++){
        int newI = FireColorTable[baseColor + (i << 8)];
        result.append(basePalette[newI]);
    }
    return result;
}
