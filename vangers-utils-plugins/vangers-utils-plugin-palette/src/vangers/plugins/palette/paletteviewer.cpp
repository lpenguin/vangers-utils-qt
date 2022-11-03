#include "paletteviewer.h"
#include <QFile>
#include <QFileInfo>
#include "ui_paletteviewer.h"

using namespace vangers::plugins::palette;
using namespace vangers::core::palette;
using namespace vangers::core::plugin;

const ResourceType PaletteViewerPlugin::PalType = {
    .name = "Vangers Pallette",
    .extensions = {"*.pal"}
};


const ResourceType PaletteViewerPlugin::PngType = {
    .name = "PNG Palette",
    .extensions = {"*.png"}
};

const ResourceType PaletteViewerPlugin::BmpType = {
	.name = "Bmp Palette",
	.extensions = {"*.bmp"}
};

const ResourceType PaletteViewerPlugin::MicrosoftPalType = {
    .name = "Microsoft Palette",
    .extensions = {"*.pal"}
};

using namespace vangers::plugins::palette;

const QMap<QString, QSharedPointer<AbstractResourceAccess<Palette>>> PaletteViewer::accesses = {
    {PaletteViewerPlugin::PalType.name, QSharedPointer<VangersPaletteAccess>::create()},
	{PaletteViewerPlugin::PngType.name, QSharedPointer<PngPaletteAccess>::create(16, "png")},
	{PaletteViewerPlugin::BmpType.name, QSharedPointer<PngPaletteAccess>::create(256, "bmp")},
	{PaletteViewerPlugin::MicrosoftPalType.name, QSharedPointer<MicrosoftPalAccess>::create()},
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
	Palette::store(ui->paletteGrid->vangersPalette(), basename);
}

QList<ResourceType> PaletteViewerPlugin::supportedImportTypes() const
{
    return {PalType, PngType, MicrosoftPalType};
}

QList<ResourceType> PaletteViewerPlugin::supportedExportTypes() const
{
	return {PalType, BmpType, PngType, MicrosoftPalType};
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

bool PaletteViewer::importResource(const QString& filePath, const ResourceType& resourceType)
{
    if(!accesses.contains(resourceType.name)){
        return false;
    }

    QFile f(filePath);
    f.open(QFile::ReadOnly);

    auto& access = accesses[resourceType.name];
	Palette palette;
	bool success = access->read(palette, f);
	if(!success){
		return false;
	}

    _currentFile = filePath;
	ui->paletteGrid->setPalette(palette);

    return true;
}

void PaletteViewer::exportResource(const QString& filePath, const ResourceType& resourceType)
{
    if(!accesses.contains(resourceType.name)){
        return;
    }

    QFile f(filePath);
    f.open(QFile::WriteOnly);
    auto& palette = ui->paletteGrid->vangersPalette();

    auto& access = accesses[resourceType.name];
	access->write(palette, f);
}

PaletteViewer::~PaletteViewer()
{
    delete ui;
}
