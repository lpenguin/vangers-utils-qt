#ifndef PALETTE_H
#define PALETTE_H
#include <QVector>
#include <QRgb>
#include <QIODevice>


namespace vangers {
    class Palette: public QVector<QRgb>
    {
    public:
        static Palette read(QIODevice& device);
        static Palette read(const QString& name);
        static Palette grayscale();
    };

}

#endif // PALETTE_H
