#ifndef PALETTE_H
#define PALETTE_H
#include <QVector>
#include <QRgb>
#include <QIODevice>


namespace vangers {
    class Palette: public QVector<QRgb>
    {
    public:
        static Palette read(const QString& name);
        static QStringList paletteNames();
        static Palette grayscale();
        static Palette fire();
        static Palette fireball(const Palette &basePalette, unsigned char baseColor = 0);
        static Palette read(QIODevice& device);
        static void store(const Palette& palette, const QString& name);
        static void store(const Palette& palette, QIODevice& device);
    };

}

#endif // PALETTE_H
