#ifndef PALETTEGRID_H
#define PALETTEGRID_H

#include <QWidget>
#include "image/palette.h"

namespace Ui {
class PaletteGrid;
}

class PaletteGrid : public QWidget
{
    Q_OBJECT

public:
    explicit PaletteGrid(QWidget *parent = nullptr);
    ~PaletteGrid();

    void setPalette(const vangers::Palette& palette);

    const vangers::Palette& vangersPalette() const {
        return _palette;
    }
private:
    vangers::Palette _palette;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void colorSelected(int colorIndex);
};

#endif // PALETTEGRID_H
