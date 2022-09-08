#include "vmapviewerplugin.h"
#include "ui_vmapviewer.h"
#include "vmapreader.h"
#include "vmapiniwriter.h"
#include "vmapmeta.h"
#include "vmapvoxwriter.h"

#include <QSettings>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QImage>
#include <QAbstractButton>

#include <splay/splay.h>
#include <vangers/core//palette/palette.h>
#include "layer/terrainimagelayer.h"
#include "layer/allimagelayer.h"
#include "layer/isshadowedimagelayer.h"
#include "layer/doublelevelimagelayer.h"
#include "layer/deltaimagelayer.h"
#include "layer/heightimagelayer.h"
#include "layer/floodlayer.h"
#include "voxwritersettingsdialog.h"


using namespace vangers::core::plugin;
using namespace vangers::core::palette;
using namespace vangers::plugins::vmap;

const ResourceType VmapViewerPlugin::VmcType = {
    .name = "Vangers level",
    .extensions = {"*.ini"}
};

const ResourceType VmapViewerPlugin::LevelType = {
    .name = "Uncompressed level directory",
    .extensions = {"*.ini"}
};

const ResourceType VmapViewerPlugin::VoxType = {
	.name = "MagicaVoxel VOX",
	.extensions = {"*.vox"}
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
								 "Flood",
							 });

	_layers = {
		{"Terrain", new TerrainImageLayer(this)},
		{"All", new AllImageLayer(this)},
		{"Shadow", new IsShadowedImageLayer(this)},
		{"DoubleLevel", new DoubleLevelImageLayer(this)},
		{"Delta", new DeltaImageLayer(this)},
		{"Height", new HeightImageLayer(this)},
		{"Flood", new FloodLayer(this)},
	};

	QObject::connect(_ui->maskCombo, SIGNAL(currentIndexChanged(int)),
					 this, SLOT(onMaskTypeChanged(int)));
	QObject::connect(_ui->graphicsView, &ImageGraphicsView::mouseMove,
					 this, &VmapViewer::onMapMouseMove);
	QObject::connect(_ui->mixSlider, &QSlider::valueChanged,
					 this, &VmapViewer::onMixValueChanged);
	QObject::connect(_ui->levelComboBox, SIGNAL(currentIndexChanged(int)),
					 this, SLOT(onLevelChanged(int)));
}

VmapViewer::~VmapViewer()
{

}

bool VmapViewer::importResource(const QString &filename, const ResourceType &resourceType)
{
    _currentFile = filename;

	_vmap = QSharedPointer<Vmap>::create();
	VmapReader reader;
	QFile file(filename);
	file.open(QFile::ReadOnly);
	if(!reader.read(*_vmap, file)){
		qDebug() << "Cannot read file" << filename;
		file.close();
		return false;
	}
	file.close();

	auto* old = _ui->graphicsView->scene();
	if(old != nullptr){
		old->clear();
		delete old;
	}
//    _ui->graphicsView
    QGraphicsScene* scene = new QGraphicsScene(this);
	_ui->graphicsView->setScene(scene);
    {
		QSharedPointer<QImage> image = _layers["Height"]->getImage(*_vmap, getLevel());
		QPixmap pixmap = QPixmap::fromImage(*image);
		_heightItem = scene->addPixmap(pixmap);
		scene->setSceneRect(image->rect());
    }

	{
		QSharedPointer<QImage> image = _layers["All"]->getImage(*_vmap, getLevel());
		QPixmap pixmap = QPixmap::fromImage(*image);
		_metaItem = scene->addPixmap(pixmap);
		_metaItem->setVisible(false);
	}

//    item->setTransformationMode(Qt::SmoothTransformation);
//    qDebug() << i.rect() << _image->image()->rect() << _image->image();



    _ui->heightButton->setChecked(true);
    _ui->metaButton->setChecked(false);

	_ui->maskCombo->setCurrentIndex(0);
	_ui->mixSlider->setValue(100);
    return true;

}

void VmapViewer::exportResource(const QString &filename, const ResourceType & type)
{
    if(_vmap.isNull()){
        return;
    }

	if(type.name == VmapViewerPlugin::LevelType.name){
		VmapIniWriter writer;
		writer.write(*_vmap, filename, getLevel());
	} else if(type.name == VmapViewerPlugin::VoxType.name){
		VmapVoxWriter writer;
		auto settings = VmapVoxWriterSettings::makeDefault();
		settings.voxSizeX = 512;
		settings.voxSizeY = 512;
		VoxWriterSettingsDialog settingsDialog(this);
		int result = settingsDialog.exec();
		if(result == QDialog::Accepted){
			settings = settingsDialog.getSettings();
			writer.write(*_vmap, filename, settings);
		}

	}

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

	QSharedPointer<QImage> metaImage = _layers[layerName]->getImage(*_vmap, getLevel());
	QPixmap pixmap = QPixmap::fromImage(*metaImage);
	_metaItem->setPixmap(pixmap);
}

Level VmapViewer::getLevel()
{
	int currentIndex = _ui->levelComboBox->currentIndex();
	if(currentIndex < 0 || currentIndex > (int)Level::MAX_LEVEL){
		return Level::Invalid;
	}
	return (Level)currentIndex;
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
	if(checked){
		float mix = (float)_ui->mixSlider->value() / 100.0f;
		_metaItem->setOpacity(mix);
		_heightItem->setOpacity(1 - mix);
	}else{
		_heightItem->setOpacity(1);
	}
}

void VmapViewer::onMaskTypeChanged(int maskIndex)
{
	applyMask(_ui->maskCombo->itemText(maskIndex));
}

void VmapViewer::onMapMouseMove(QPointF pos)
{
	int x = pos.x();
	int y = pos.y();
	QSize size = _vmap->size();
	if(x < 0 || x >= size.width() || y < 0 || y >= size.height()){
		return;
	}
	uint8_t height = _vmap->height().getData(x, y);

	uint8_t meta = _vmap->meta().getData(x, y);
	VmapMeta vmapMeta = VmapMeta::fromMeta(meta);

	int32_t evenX, oddX;
	if(x % 2 == 0) {
		evenX = x;
		oddX = x + 1;
	} else {
		evenX = x - 1;
		oddX = x;
	}

	int32_t evenDelta = VmapMeta::fromMeta(_vmap->meta().getData(evenX, y)).delta();
	int32_t oddDelta = VmapMeta::fromMeta(_vmap->meta().getData(oddX, y)).delta();
	int32_t delta = (evenDelta << 2) + oddDelta;

	int32_t floodChunkSize = _vmap->size().height() / _vmap->floodConst().size();
	int32_t floodChunk = y / floodChunkSize;
	int32_t flood = -1;

	if(floodChunk >= 0 && floodChunk < _vmap->floodConst().size()){
		flood = _vmap->floodConst()[floodChunk];
	}
	_ui->statusLabel->setText(QString("%1x%2 alt: %3, terrain: %4, delta: %7 (%9 %10), isDoubleLevel: %5, isShadowed: %6, flood: %8 ")
							  .arg(x)
							  .arg(y)
							  .arg(height)
							  .arg(vmapMeta.terrain())
							  .arg(vmapMeta.isDoubleLevel())
							  .arg(vmapMeta.isShadowed())
							  .arg(delta)
							  .arg(flood)
							  .arg(evenDelta)
							  .arg(oddDelta));
}

void VmapViewer::onMixValueChanged(int value)
{
	if(_metaItem->isVisible()){
		float mix = (float)value / 100.0f;
		_metaItem->setOpacity(mix);
		_heightItem->setOpacity(1 - mix);
	} else {
		_heightItem->setOpacity(1.0f);
	}
}

void VmapViewer::onLevelChanged(int index)
{
	onMaskTypeChanged(_ui->maskCombo->currentIndex());

	auto* scene = _ui->graphicsView->scene();

	QSharedPointer<QImage> image = _layers["Height"]->getImage(*_vmap, getLevel());
	QPixmap pixmap = QPixmap::fromImage(*image);
	_heightItem->setPixmap(pixmap);
}


ResourceViewer *VmapViewerPlugin::makeResourceViewer(QWidget *parent)
{
    return new VmapViewer(this, parent);
}
