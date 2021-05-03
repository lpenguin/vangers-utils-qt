#include "paletteviewer.h"
#include "ui_paletteviewer.h"
#include "image/palette.h"

#include <QDir>
#include <qstringlistmodel.h>
#include <QDebug>

PaletteViewer::PaletteViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaletteViewer)
{
    ui->setupUi(this);

    QStringListModel* palettesModel = new QStringListModel(QDir(":/palettes/pal").entryList(), this);
    ui->paletteList->setModel(palettesModel);
    QItemSelectionModel* selectionModel = ui->paletteList->selectionModel();

    QObject::connect(selectionModel, &QItemSelectionModel::currentChanged, this, &PaletteViewer::palette_currentChanged);
    ui->paletteList->setCurrentIndex(palettesModel->index(0));

    QObject::connect(ui->paletteGrid, &PaletteGrid::colorSelected, this, &PaletteViewer::paletteGrid_colorSelected);
    QObject::connect(ui->useTransparentCheckbox, &QCheckBox::clicked, this, &PaletteViewer::transparentCheckbox_clicked);

}

void PaletteViewer::palette_currentChanged(const QModelIndex &current, const QModelIndex &)
{
    QString paletteName = ui->paletteList->model()->data(current).toString();
    vangers::Palette pal = vangers::Palette::read(paletteName);
    ui->paletteGrid->setPalette(pal);
    emit paletteChanged(paletteName);
}

void PaletteViewer::paletteGrid_colorSelected(int colorIndex)
{
    ui->transparentEditor->setText(QString("%1").arg(colorIndex));
    if(ui->useTransparentCheckbox->isChecked()){
        emit useTransparentColor(true, colorIndex);
    }

}

void PaletteViewer::transparentCheckbox_clicked(bool value)
{
    emit useTransparentColor(value, ui->transparentEditor->text().toInt());
}


PaletteViewer::~PaletteViewer()
{
    delete ui;
}

QString PaletteViewer::currentPalette() const
{
    return ui->paletteList->model()->data(ui->paletteList->currentIndex()).toString();
}
