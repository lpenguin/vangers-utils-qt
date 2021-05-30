#ifndef PALETTEVIEWER_H
#define PALETTEVIEWER_H

#include <QWidget>

namespace Ui {
class PaletteViewer;
}

class PaletteViewer : public QWidget
{
    Q_OBJECT

public:
    explicit PaletteViewer(QWidget *parent = nullptr);
    ~PaletteViewer();
    QString currentPalette() const;
signals:
    void paletteChanged(QString paletteName);
    void useTransparentColor(bool use, int index);
private slots:
    void palette_currentChanged(const QModelIndex & current, const QModelIndex & previous);
    void paletteGrid_colorSelected(int colorIndex);
    void transparentCheckbox_clicked(bool value);
private:
    Ui::PaletteViewer *ui;
};

#endif // PALETTEVIEWER_H
