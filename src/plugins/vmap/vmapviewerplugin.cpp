#include "vmapviewerplugin.h"
#include "ui_vmapviewer.h"
#include "vmapreader.h"
#include "vmapwriter.h"
#include "vmapmeta.h"

#include <QSettings>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QImage>
#include <QAbstractButton>

#include <splay/splay.h>
#include "../../image/palette.h"
#include "layer/terrainimagelayer.h"
#include "layer/allimagelayer.h"
#include "layer/isshadowedimagelayer.h"
#include "layer/doublelevelimagelayer.h"
#include "layer/deltaimagelayer.h"

using namespace vangers;

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
		{"Terrain", new TerrainImageLayer(this)},
		{"All", new AllImageLayer(this)},
		{"Shadow", new IsShadowedImageLayer(this)},
		{"DoubleLevel", new DoubleLevelImageLayer(this)},
		{"Delta", new DeltaImageLayer(this)},
	};

	QObject::connect(_ui->maskCombo, SIGNAL(currentIndexChanged(int)),
					 this, SLOT(onMaskTypeChanged(int)));

	QObject::connect(_ui->graphicsView, &ImageGraphicsView::mouseMove,
					 this, &VmapViewer::onMapMouseMove);
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
		const uchar* buf = _vmap->heightConst().data();

		int sizeX = _vmap->size().width();
		int sizeY = _vmap->size().height();
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

	VmapWriter writer;
	writer.write(*_vmap, filename);
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

void VmapViewer::onMapMouseMove(QPointF pos)
{
	qDebug() << pos;
	int x = pos.x();
	int y = pos.y();
	QSize size = _vmap->size();
	if(x < 0 || x >= size.width() || y < 0 || y >= size.height()){
		return;
	}
	uint8_t height = _vmap->height().getData(x, y);

	uint8_t meta = _vmap->meta().getData(x, y);
	VmapMeta vmapMeta = VmapMeta::fromMeta(meta);
	_ui->statusLabel->setText(QString("%1x%2 alt: %3, terrain: %4, delta: %7, isDoubleLevel: %5, isShadowed: %6 ")
							  .arg(x)
							  .arg(y)
							  .arg(height)
							  .arg(vmapMeta.terrain())
							  .arg(vmapMeta.isDoubleLevel())
							  .arg(vmapMeta.isShadowed())
							  .arg(vmapMeta.delta()));
}

ResourceViewer *VmapViewerPlugin::makeResourceViewer(QWidget *parent)
{
    return new VmapViewer(this, parent);
}
