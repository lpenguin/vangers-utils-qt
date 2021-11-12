#include "vmapviewerplugin.h"
#include "ui_vmapviewer.h"
#include "vmapaccess.h"

#include <QSettings>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QImage>
#include <QAbstractButton>
#include <graphicsviewzoom.h>

#include <splay/splay.h>
#include "../../image/palette.h"
#include "layer/terrainimagelayer.h"

const ResourceType VmapViewerPlugin::VmcType = {
    .name = "Vangers level",
    .extensions = {"*.ini"}
};

const ResourceType VmapViewerPlugin::LevelType = {
    .name = "Uncompressed level directory",
    .extensions = {"*.ini"}
};

VmapViewer::VmapViewer(VmapViewerPlugin *plugin, QWidget *parent)
    : ResourceViewer(plugin, parent)
    , _ui(new Ui::VmapViewer())
    , _currentFile()
    , _vmap()
    , _heightItem(nullptr)
    , _metaItem(nullptr)
	, _layers()
{
    _ui->setupUi(this);
    auto* gz = new GraphicsViewZoom(_ui->graphicsView);
    gz->set_modifiers(Qt::NoModifier);
    QObject::connect(_ui->heightButton, &QAbstractButton::toggled,
                     this, &VmapViewer::onHeightToggled);

    QObject::connect(_ui->metaButton, &QAbstractButton::toggled,
                     this, &VmapViewer::onMetaToggled);


	_ui->maskCombo->clear();
	_ui->maskCombo->addItems({
								 "All",
								 "Delta",
								 "Terrain",
								 "DoubleLevel",
								 "Shadow",
							 });

	_layers = {
		{"Terrain", new TerrainImageLayer(this)}
	};

	QObject::connect(_ui->maskCombo, SIGNAL(currentIndexChanged(int)),
					 this, SLOT(onMaskTypeChanged(int)));

}

VmapViewer::~VmapViewer()
{

}

bool VmapViewer::importResource(const QString &filename, const ResourceType &resourceType)
{
    _currentFile = filename;
    QSettings settings(filename, QSettings::IniFormat);
    int mapPowerX = settings.value("Global Parameters/Map Power X").toInt();
    int mapPowerY = settings.value("Global Parameters/Map Power Y").toInt();

    int sizeX = 1 << mapPowerX;
    int sizeY = 1 << mapPowerY;

	QStringList beginColorsStrs = settings.value("Rendering Parameters/Begin Colors")
			.toString()
			.trimmed()
			.split(QRegExp("\\s+"));

	if(beginColorsStrs.size() != 8){
		qWarning() << "Invalid begin colors string" << beginColorsStrs;
		return {};
	}

	QStringList endColorsStrs = settings.value("Rendering Parameters/End Colors")
			.toString()
			.trimmed()
			.split(QRegExp("\\s+"));

	if(endColorsStrs.size() != 8){
		qWarning() << "Invalid end colors string" << endColorsStrs;
		return {};
	}

	std::vector<std::pair<int, int>> terrainColorOffsets;
	for(int i = 0 ; i < beginColorsStrs.size(); i++)
	{
		int beginOffset = beginColorsStrs[i].toInt();
		int endOffset = endColorsStrs[i].toInt();
		terrainColorOffsets.push_back({beginOffset, endOffset});
	}

	QFileInfo fileInfo(filename);
	QDir fileDir(fileInfo.absoluteDir());

	QString paletteName = settings.value("Storage/Palette File").toString();
	QString paletteFileName = fileDir.filePath(paletteName);
	QFileInfo paletteFileInfo(paletteFileName);
	if(!paletteFileInfo.exists()){
		qWarning() << "Palette doesn't exist" << paletteFileName;
		return {};
	}

	QFile paletteFile(paletteFileName);
	paletteFile.open(QFile::ReadOnly);
	vangers::Palette pal = vangers::Palette::read(paletteFile);
	paletteFile.close();


    QString levelBaseName = settings.value("Storage/File Name").toString();



	QString vmcFileName = QDir(fileDir).filePath(levelBaseName + ".vmc");
    QFileInfo vmcFileInfo = vmcFileName;

    if(!vmcFileInfo.exists()){
        qWarning() << "VMC file doesn't exsit" << vmcFileName;
        return false;
    }

    QFile vmcFile(vmcFileName);
    if(!vmcFile.open(QFile::ReadOnly)){
        qWarning() << "Cannot open file for reading" << vmcFileName;
        return false;
    }

	VmapAccess access(sizeX, sizeY, pal, terrainColorOffsets);
    _vmap = access.read(vmcFile);

//    _ui->graphicsView
    QGraphicsScene* scene = new QGraphicsScene(this);
    {
        uchar* buf = _vmap->height().data();

        QImage image(buf, sizeX, sizeY, sizeX, QImage::Format_Indexed8);

        image.setColorTable(vangers::Palette::grayscale());


        QPixmap pixmap = QPixmap::fromImage(image);
        _heightItem = scene->addPixmap(pixmap);
        scene->setSceneRect(image.rect());
    }

    {
		const auto& meta = _vmap->meta();
		uchar* buf = new uchar[meta.size()];
		for(size_t i =0; i < meta.size(); i++){
			buf[i] = meta[i];
		}

        QImage image(buf, sizeX, sizeY, sizeX, QImage::Format_Indexed8);

        image.setColorTable(vangers::Palette::grayscale());


        QPixmap pixmap = QPixmap::fromImage(image);
        _metaItem = scene->addPixmap(pixmap);
        _metaItem->setVisible(false);
    }

//    item->setTransformationMode(Qt::SmoothTransformation);
//    qDebug() << i.rect() << _image->image()->rect() << _image->image();

    _ui->graphicsView->setScene(scene);

    _ui->heightButton->setChecked(true);
    _ui->metaButton->setChecked(false);

	_ui->maskCombo->setCurrentIndex(0);

    return true;

}

void VmapViewer::exportResource(const QString &filename, const ResourceType &)
{
    if(_vmap.isNull()){
        return;
    }

    QSettings settings(filename, QSettings::IniFormat);
    QDir baseDir(QFileInfo(filename).absoluteDir());
    QString heightFilename = baseDir.filePath("height.png");
    QString metaFilename = baseDir.filePath("meta.png");

    settings.setValue("height", heightFilename);
    settings.setValue("meta", metaFilename);
    settings.sync();

    _vmap->heightImage()->save(heightFilename);
    _vmap->metaImage()->save(metaFilename);
}

QString VmapViewer::currentFile() const
{
    return _currentFile;
}

void VmapViewer::applyMask(QString layerName)
{
	if(!_layers.contains(layerName)){
		qWarning() << "No layer definition" << layerName;
		return;
	}

	QSharedPointer<QImage> metaImage = _layers[layerName]->getImage(*_vmap);
	QPixmap pixmap = QPixmap::fromImage(*metaImage);
	_metaItem->setPixmap(pixmap);
}

void VmapViewer::onHeightToggled(bool checked)
{
    if(_heightItem == nullptr){
        return;
    }

    _heightItem->setVisible(checked);
}

void VmapViewer::onMetaToggled(bool checked)
{
    if(_metaItem == nullptr){
        return;
    }

	_metaItem->setVisible(checked);
}

void VmapViewer::onMaskTypeChanged(int maskIndex)
{
	applyMask(_ui->maskCombo->itemText(maskIndex));
}


ResourceViewer *VmapViewerPlugin::makeResourceViewer(QWidget *parent)
{
    return new VmapViewer(this, parent);
}
