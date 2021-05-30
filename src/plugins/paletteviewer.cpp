#include "paletteviewer.h"
#include "ui_paletteviewer.h"

#include <QFile>
#include <QFileInfo>


const ResourceType PaletteViewerPlugin::PalType = {
    .name = "Vangers Pallette",
    .extensions = {"*.pal"}
};


const ResourceType PaletteViewerPlugin::PngType = {
    .name = "PNG Palette",
    .extensions = {"*.png"}
};

QString PaletteViewer::currentFile() const
{
    return _currentFile;
}

void PaletteViewer::importButton_click()
{
    QFileInfo info(_currentFile);
    if(!info.exists()){
        return;
    }

    auto basename = info.baseName();
    vangers::Palette::store(ui->paletteGrid->vangersPalette(), basename);
}

QList<ResourceType> PaletteViewerPlugin::supportedImportTypes() const
{
    return {PalType, PngType};
}

QList<ResourceType> PaletteViewerPlugin::supportedExportTypes() const
{
    return {PalType, PngType};
}

ResourceViewer* PaletteViewerPlugin::makeResourceViewer(QWidget* parent)
{
    return new PaletteViewer(this, parent);
}


PaletteViewer::PaletteViewer(ResourceViewerPlugin* plugin, QWidget *parent) :
    ResourceViewer(plugin, parent),
    ui(new Ui::PaletteViewer)
{
    ui->setupUi(this);
    ui->verticalLayout->setAlignment(Qt::AlignTop);
    QObject::connect(ui->importPaletteButton, &QPushButton::clicked, this, &PaletteViewer::importButton_click);
}

void PaletteViewer::importResource(const QString& filePath, const ResourceType& resourceType)
{
    _currentFile = filePath;

    if(resourceType.name == PaletteViewerPlugin::PalType.name){
        QFile paletteFile(filePath);
        paletteFile.open(QFile::ReadOnly);
        vangers::Palette pal = vangers::Palette::read(paletteFile);
        ui->paletteGrid->setPalette(pal);
        return;
    }

    if(resourceType.name == PaletteViewerPlugin::PngType.name){
        QImage image(filePath);
        vangers::Palette pal;
        for(int ix = 0; ix < image.width(); ix ++){
            for(int iy = 0; iy < image.height(); iy++){
                pal << image.pixel(ix, iy);
            }
        }
        ui->paletteGrid->setPalette(pal);
        return;
    }

}

void PaletteViewer::exportResource(const QString& filePath, const ResourceType& resourceType)
{
    if(resourceType.name == PaletteViewerPlugin::PalType.name){
        const auto& palette = ui->paletteGrid->vangersPalette();
        QFile f(filePath);
        f.open(QFile::WriteOnly);
        vangers::Palette::store(palette, f);
    }

    if(resourceType.name == PaletteViewerPlugin::PngType.name){
        const auto& palette = ui->paletteGrid->vangersPalette();
        int nCols = 16;
        int nRows = palette.size() / nCols;
        QImage image(nCols, nRows, QImage::Format_RGB32);
        for(int i = 0; i < palette.size(); i++){
            int iCol = i % nCols;
            int iRow = i / nCols;
            image.setPixel(iCol, iRow, palette[i]);
        }
        image.save(filePath);
    }
}

PaletteViewer::~PaletteViewer()
{
    delete ui;
}
