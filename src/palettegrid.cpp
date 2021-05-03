#include "palettegrid.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

PaletteGrid::PaletteGrid(QWidget *parent) :
    QWidget(parent)
{

}

PaletteGrid::~PaletteGrid()
{

}

void PaletteGrid::setPalette(const vangers::Palette &palette)
{
    _palette = palette;
    QWidget::update();
}

void PaletteGrid::paintEvent(QPaintEvent *event)
{
    if(_palette.size() == 0){
        return;
    }

    QRect rect = this->rect();
    const int nCols = 16;
    const int nRows = _palette.size() / nCols;
    const int sx = rect.width() / nCols;
    const int sy = rect.height() / nRows;

    QPainter painter(this);
    int iColor = 0;
    for(int iy = 0; iy < nRows; iy++){
        for(int ix = 0; ix < nCols; ix ++, iColor++){
            if(iColor >= _palette.size()){
                return;
            }
            QRgb color = _palette[iColor];
            QRect colorRect(rect.topLeft() + QPoint(ix * sx, iy * sy), QSize(sx, sy));
            painter.setBrush(QBrush(color));
            painter.drawRect(colorRect);
        }
    }
}

void PaletteGrid::mousePressEvent(QMouseEvent *event)
{
    if(_palette.size() == 0){
        return;
    }

    QRect rect = this->rect();
    const int nCols = 16;
    const int nRows = _palette.size() / nCols;
    const int sx = rect.width() / nCols;
    const int sy = rect.height() / nRows;
    int col = event->pos().x()/sx;
    int row = event->pos().y()/sy;
    int index = row * nCols + col;
    qDebug() << col << row <<index;
    emit colorSelected(index);
}
