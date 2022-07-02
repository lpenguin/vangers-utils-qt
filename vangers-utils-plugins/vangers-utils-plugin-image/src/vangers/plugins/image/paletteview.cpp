#include "paletteview.h"
#include "ui_paletteview.h"
#include <vangers/core//palette/palette.h>

#include <QDir>
#include <qstringlistmodel.h>
#include <QDebug>

using namespace vangers::core::palette;
using namespace vangers::plugins::image;

PaletteView::PaletteView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaletteView)
{
    ui->setupUi(this);

	QStringListModel* palettesModel = new QStringListModel(Palette::paletteNames(), this);
    ui->paletteList->setModel(palettesModel);
    QItemSelectionModel* selectionModel = ui->paletteList->selectionModel();

    QObject::connect(selectionModel, &QItemSelectionModel::currentChanged, this, &PaletteView::palette_currentChanged);
    ui->paletteList->setCurrentIndex(palettesModel->index(0));

    QObject::connect(ui->paletteGrid, &PaletteGrid::colorSelected, this, &PaletteView::paletteGrid_colorSelected);
    QObject::connect(ui->useTransparentCheckbox, &QCheckBox::clicked, this, &PaletteView::transparentCheckbox_clicked);

}

void PaletteView::palette_currentChanged(const QModelIndex &current, const QModelIndex &)
{
    QString paletteName = ui->paletteList->model()->data(current).toString();
	Palette pal = Palette::read(paletteName);
    ui->paletteGrid->setPalette(pal);
    emit paletteChanged(paletteName);
}

void PaletteView::paletteGrid_colorSelected(int colorIndex)
{
    ui->transparentEditor->setText(QString("%1").arg(colorIndex));
    if(ui->useTransparentCheckbox->isChecked()){
        emit useTransparentColor(true, colorIndex);
    }

}

void PaletteView::transparentCheckbox_clicked(bool value)
{
    emit useTransparentColor(value, ui->transparentEditor->text().toInt());
}


PaletteView::~PaletteView()
{
    delete ui;
}

QString PaletteView::currentPalette() const
{
    return ui->paletteList->model()->data(ui->paletteList->currentIndex()).toString();
}

void PaletteView::setEnabled(bool enabled)
{
    ui->paletteList->setEnabled(enabled);
}
